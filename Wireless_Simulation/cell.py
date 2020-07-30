#!/usr/bin/env python
# coding: utf-8

import random
import time
from collections import namedtuple
import matplotlib.pyplot as plt
import numpy as np

class User():
    """class for users in cellular network simulation

    :param uid: 
    :type uid:

    :param uid: 
    :type uid:

    :param uid: 
    :type uid:

    :param uid: 
    :type uid:

    :param uid: 
    :type uid:

    :param uid: 
    :type uid:

    :param uid: 
    :type uid:

    :param uid: 
    :type uid:
    """
    def __init__(self, uid, is_road, is_finished_shopping):
        self.uid = uid
        self.loc = -1
        self.cell = -1
        self.is_calling = False
        self.call_time = -1
        self.channel = -1
        self.is_road = is_road
        self.shop_time = -1
        self.is_finished_shopping = is_finished_shopping

    def init_loc(self, basic_param):
        """initialize location of a user when entering road or shop

        :param basic_param: The path to the folder where you want to load your .json files
        :type basic_param: str
        """
        
        if self.is_road == True:

            # uniformly distributed along the road
            self.loc = np.random.randint(-basic_param.road_len//2, basic_param.road_len//2)

        if self.is_road == False:

            # uniformly located in a circle of radius
            self.loc = np.random.randint(low=1, high=basic_param.shop_rad)

            # uniformly distributed shop time
            self.shop_time = np.random.randint(low=1, high=basic_param.shop_time_mu)

    def update_loc(self, user_param):
        """update location of a user on the road at every time step

        :param json_dir: The path to the folder where you want to load your .json files
        :type json_dir: str
        """
        
        # if user is going back to home
        if self.is_finished_shopping == True:
            self.loc = self.loc + np.sign(self.loc) * user_param.mobile_speed

        # if user is heading to the shopping mall
        if self.is_finished_shopping == False:
            self.loc = self.loc - np.sign(self.loc) * user_param.mobile_speed

    def set_handoff(self, channel, cell):
        """ perform handoff by freeing channel from one cell and assign channel to a new cell
        """
        # user will occupy it and free the channel on the macro cell
        self.free_channel(channel)

        # assign new channel on the small to user
        self.assign_channel(channel, cell)

    def set_call(self, channel, cell, user_param):
        """ set call by assigning a channel to user and setting call time
        """
        # connect user to the cell
        self.assign_channel(channel, cell)

        # determine length of the call
        self.call_time = np.random.exponential(user_param.call_dur)

    def assign_channel(self, channel, cell):
        """assign available channel in a cell to a user
        """                             
        # allocate new channel in cell
        new_channel = channel[cell].index(0)
        channel[cell][new_channel] = self.uid

        # update the active call records
        self.channel = new_channel
        self.cell = cell
        self.is_calling = True

    def free_channel(self, channel):
        """free the channel when call is completed, dropped in active or handoff is made
        """
        channel[self.cell][self.channel] = 0
        self.channel = -1
        self.cell = -1
        self.is_calling = False

    def is_entered_shop(self, basic_param, user_param):
        """check if user will enter at the shopping mall
        """
        # 75% of prob that a user will enter the shop
        if np.random.uniform(0, 1) <= basic_param.p:
            self.is_road = False
            self.loc = np.random.randint(low=1, high=basic_param.shop_rad)
            self.shop_time = np.random.normal(1800, 200)

            return True

        # 25% that a user will just pass by to go home
        else:
            self.update_loc(user_param)
            self.is_finished_shopping = True

            return False

    def is_finished_shop(self, basic_param):
        """check whether finished shopping or not
        """
        # go back to road 
        self.is_road = True
        self.is_finished_shopping = True
        self.shop_time = None

        # 50% chance of being on the west or east of the shopping mall
        if np.random.normal(0,1) < 0.5:
            self.loc = basic_param.enter_thresh

        else:
            self.loc = -basic_param.enter_thresh

    def is_left_sim(self, basic_param):
        """check whether the user leaves either side of the highway
        """
        if abs(self.loc) >= basic_param.road_len/2:
            return True

        else:
            return False

    def replace_new_user(self, basic_param):
        self.call_time = -1
        self.channel = -1
        self.cell = -1
        self.is_calling = False
        self.is_road = True
        self.shop_time = -1
        self.is_finished_shopping = False
        self.init_loc(basic_param)


def takeSecond(elem):
    return elem.is_calling


def run_sim(basic_param, base_param, user_param, macro_cell_stat, small_cell_stat):
    """run simulation for a given time
    """

    # keep track of channels for each cell,
    channel = {'macro':[0]*base_param.macro_num_channels, 'small':[0]*base_param.small_num_channels}

    # aggregates important stats per time step,

    # initialize users throughout the area
    users, shadowing = init_sim(basic_param, user_param)
    print('Initialized users and fixed values')

    # for every time step = 1 sec
    for time in range(basic_param.total_time):

        if time % 60 == 0:
            print('{} min'.format(time/60))

        users.sort(key=takeSecond, reverse=True)

        # update the users' positions, shopping times, call times
        for user in users:

            # update users' positions on the road
            if user.is_road == True:
                user.update_loc(user_param)

                # check if user arrived at the shopping mall
                if abs(user.loc) < basic_param.enter_thresh:
                    user.is_entered_shop(basic_param, user_param)

            # update shopping time of users
            else:
                user.shop_time -= 1
                
                # if user finished shopping
                if user.shop_time <= 0:
                    user.is_finished_shop(basic_param)

            # first serve active users
            if user.is_calling == True:
                user.call_time -= 1        

                # case 1: call was successfully completed by leaving the simulation
                if user.is_left_sim(basic_param):
                    
                    # update completed call on the serving cell
                    if user.cell == 'macro':
                        update_counter(macro_cell_stat, 'completed_calls', time)

                    if user.cell == 'small':
                        update_counter(small_cell_stat, 'completed_calls', time)

                    # delete the active call entry in the table and free the channel
                    user.free_channel(channel)    

                    # if user left the simulation, replace the user
                    user.replace_new_user(basic_param)

                # case 2: call was successfully completed by leaving the simulation
                elif user.call_time < 0:
                    
                    # update completed call on the serving cell
                    if user.cell == 'macro':
                        update_counter(macro_cell_stat, 'completed_calls', time)

                    if user.cell == 'small':
                        update_counter(small_cell_stat, 'completed_calls', time)
                    
                    # delete the active call entry in the table and free the channel
                    user.free_channel(channel)
                    user.call_time = -1

                # case 3: active call is dropped or is still on
                else:
                    rsl = calculate_rsl(user, basic_param, base_param, user_param, shadowing)

                    # active call is dropped 
                    if rsl[user.cell] < user_param.rx_thresh:
                        print('call dropped while in active')

                        # update dropped call count on the serving cell
                        if user.cell == 'macro':
                            update_counter(macro_cell_stat, 'dropped_calls', time) 
                            
                        if user.cell == 'small':
                            update_counter(small_cell_stat, 'dropped_calls', time)

                        # delete the active call entry in the table and free the channel
                        user.free_channel(channel)

                    # call is still on (rsl[user.cell] > threshold)
                    else:
                        # if the call is on the small cell
                        if user.cell == 'small':

                            # the call is continued on the small cell
                            if rsl['small'] >= user_param.small_thresh:
                                pass

                            # check potential handoff from small to macro,
                            elif rsl['macro'] >= (rsl['small'] + user_param.HO_margin):

                                # record this as handoff attempt from small to macro,
                                update_counter(small_cell_stat, 'HO_attempt_s2m', time)

                                # if macro cell has a channel available
                                if 0 in channel['macro']:
                                    user.set_handoff(channel, 'macro')
                                    update_counter(small_cell_stat, 'HO_success_s2m', time)

                                # recorded as handoff failure and capacity block for macro cell
                                else:
                                    update_counter(small_cell_stat, 'HO_failure_s2m', time)
                                    update_counter(macro_cell_stat, 'num_cap_blocks', time)
                            else:
                                pass

                        # if the call is on the macro cell
                        elif user.cell == 'macro':

                            # check potential handoff from macro to small
                            if rsl['small'] > user_param.small_thresh or rsl['small'] >= (rsl['macro']+user_param.HO_margin):

                                # record this as a handoff attempt
                                update_counter(macro_cell_stat, 'HO_attempt_m2s', time)

                                # if the small cell has a channel available 
                                if 0 in channel['small']:     
                                    user.set_handoff(channel, 'small')
                                    update_counter(macro_cell_stat, 'HO_success_m2s', time)

                                # recorded as handoff failure and capacity block by small cell
                                else:
                                    update_counter(macro_cell_stat, 'HO_failure_m2s', time)
                                    update_counter(small_cell_stat,'num_cap_blocks', time)
                        else:
                            pass

            # next serve non-active users
            elif user.is_calling == False:

                # if the user has left the road, replace them
                if user.is_left_sim(basic_param):
                    user.replace_new_user(basic_param)

                # determine if the user makes a call request
                elif np.random.uniform(0, 1) <= user_param.call_rate:                   
                    rsl = calculate_rsl(user, basic_param, base_param, user_param, shadowing)

                    # case 1 : attempt to connect to small cell first
                    if rsl['small'] > user_param.small_thresh:
                        update_counter(small_cell_stat, 'call_attempts', time)

                        # check if there is an available channel
                        if 0 in channel['small']:
                            user.set_call(channel, 'small', user_param)
                            update_counter(small_cell_stat, 'success_connections', time)

                        # capacity block for small
                        else:
                            update_counter(small_cell_stat, 'num_cap_blocks', time)

                            # try to connect to macro : check if signal strength of macro cell is enough
                            if rsl['macro'] >= user_param.rx_thresh:

                                # check if there is an available channel
                                if 0 in channel['macro']:
                                    user.set_call(channel, 'macro', user_param)
                                    update_counter(macro_cell_stat, 'success_connections', time)
                                    
                                # capacity block for macro and call drops
                                else:
                                    update_counter(small_cell_stat, 'dropped_calls', time)
                                    
                            # signal block for macro and call drops
                            else:
                                update_counter(small_cell_stat, 'dropped_calls', time)

                    # case 2: attempt to connect to small cell first
                    elif rsl['small'] >= rsl['macro']:
                        update_counter(small_cell_stat, 'call_attempts', time)
                        
                        # check if there is enough signal strength for small cell
                        if rsl['small'] >= user_param.rx_thresh:
                    
                            # check if there is an available channel
                            if 0 in channel['small']:
                                user.set_call(channel, 'small', user_param)
                                update_counter(small_cell_stat, 'success_connections', time)

                            # capacity block for small
                            else:
                                update_counter(small_cell_stat, 'num_cap_blocks', time)

                                # check if signal strength of macro cell is enough,
                                if rsl['macro'] >= user_param.rx_thresh:

                                    # check if there is an available channel,
                                    if 0 in channel['macro']:
                                        user.set_call(channel, 'macro', user_param)
                                        update_counter(macro_cell_stat, 'success_connections', time)

                                    # capacity block for macro and call drops
                                    else:
                                        update_counter(small_cell_stat, 'dropped_calls', time)
                                        
                                # signal block for macro and call drops
                                else:
                                    update_counter(small_cell_stat, 'dropped_calls', time)

                        # not enough signal for both cells
                        else:
                            update_counter(small_cell_stat, 'num_sig_blocks', time)

                            if user.is_road == True:
                                update_counter(macro_cell_stat, 'dropped_calls', time)
                            else:
                                update_counter(small_cell_stat, 'dropped_calls', time)

                    # case 3: attempt to connect to macro cell first (rsl['macro'] >= rsl['small'])
                    else:
                        update_counter(macro_cell_stat, 'call_attempts', time)
                        
                        # check if there is enough signal for macro cell
                        if rsl['macro'] >= user_param.rx_thresh:

                            # check if there is an available channel on macro cell
                            if 0 in channel['macro']:
                                user.set_call(channel, 'macro', user_param)
                                update_counter(macro_cell_stat, 'success_connections', time)

                            # if no available channel on macro cell
                            else:
                                update_counter(macro_cell_stat, 'num_cap_blocks', time)

                                # try to connect to other cell by checking if signal of small cell is enough
                                if rsl['small'] >= user_param.rx_thresh:

                                    # check if there is an available channel
                                    if 0 in channel['small']:
                                        user.set_call(channel, 'small', user_param)
                                        update_counter(small_cell_stat, 'success_connections', time)

                                    else:
                                        update_counter(macro_cell_stat, 'dropped_calls', time)
                                else:
                                    # not enough signal for small cell
                                    update_counter(macro_cell_stat, 'dropped_calls', time)

                        # not enough signal for both cells
                        else:
                            update_counter(macro_cell_stat, 'num_sig_blocks', time)

                            if user.is_road == True:
                                update_counter(macro_cell_stat, 'dropped_calls', time)

                            else:
                                update_counter(small_cell_stat, 'dropped_calls', time)
                else:
                    pass

            # update any counters left
            macro_cell_stat['num_channels'][time] = base_param.macro_num_channels - channel['macro'].count(0)
            small_cell_stat['num_channels'][time] = base_param.small_num_channels - channel['small'].count(0)

        # after each hour of simul time, produce a cell report
        if time > 0 and time % 3600 == 0:
            report_cell_stat(macro_cell_stat, small_cell_stat, time)
            
            # count number of users on the road and shopping mall
            num_road_user = 0
            num_shop_user = 0
            
            for user in users:
                if user.is_road == True:
                    num_road_user += 1
                else:
                    num_shop_user += 1
            print('number of road users: {}, number of shop users: {}'.format(num_road_user, num_shop_user))

    return macro_cell_stat, small_cell_stat


def init_sim(basic_param, user_param):
    """calculate various initial settings :the users’ positions and motion
    """

    # assign road users and mall users
    road_users = [User(uid=i+1, is_road=True, is_finished_shopping=False) for i in range(user_param.num_users//2)]
    mall_users = [User(uid=i+1+(user_param.num_users//2), is_road=False, is_finished_shopping=True) for i in range(user_param.num_users//2)]

    # combine both user list into one,
    users = road_users + mall_users

    # assign initial random location for each user,
    for i in range(user_param.num_users):
        users[i].init_loc(basic_param)

    # initialize a list for keeping shadowing values for every 5m,
    shadow = np.random.normal(0, 2, (basic_param.road_len//user_param.shadowing_res+1))

    return users, shadow


def update_counter(cell_stat, data, time):
    cell_stat[data][time] += 1


def report_cell_stat(macro_cell_stat, small_cell_stat, time):
        """report key statistics for small and macro cell at a given time
        """
        print('[small cell statistics]')
        print('the number of channels currently in use: {}'.format(small_cell_stat['num_channels'][time]))
        print('the number of call attempts: {}'.format(np.sum(small_cell_stat['call_attempts'][:time])))
        print('the number of successful call connections: {}'.format(np.sum(small_cell_stat['success_connections'][:time])))
        print('the number of successfully completed calls: {}'.format(np.sum(small_cell_stat['completed_calls'][:time])))
        print('the number of handoff attempts from small to macro: {}'.format(np.sum(small_cell_stat['HO_attempt_s2m'][:time])))
        print('the number of handoff successes from small to macro: {}'.format(np.sum(small_cell_stat['HO_success_s2m'][:time])))
        print('the number of handoff failures from small to macro: {}'.format(np.sum(small_cell_stat['HO_failure_s2m'][:time])))
        print('the number of call drops: {}'.format(np.sum(small_cell_stat['dropped_calls'][:time]))),
        print('the number of blocks due to capacity: {}'.format(np.sum(small_cell_stat['num_cap_blocks'][:time])))
        print('the number of blocks due to signal strength: {}'.format(np.sum(small_cell_stat['num_sig_blocks'][:time])))

        print('\n[macro cell statistics]') 
        print('the number of channels currently in use: {}'.format(macro_cell_stat['num_channels'][time]))
        print('the number of call attempts: {}'.format(np.sum(macro_cell_stat['call_attempts'][:time])))
        print('the number of successful call connections: {}'.format(np.sum(macro_cell_stat['success_connections'][:time])))
        print('the number of successfully completed calls: {}'.format(np.sum(macro_cell_stat['completed_calls'][:time])))
        print('the number of handoff attempts from macro to small: {}'.format(np.sum(macro_cell_stat['HO_attempt_m2s'][:time])))
        print('the number of handoff successes from macro to small: {}'.format(np.sum(macro_cell_stat['HO_success_m2s'][:time])))
        print('the number of handoff failures from macro to small: {}'.format(np.sum(macro_cell_stat['HO_failure_m2s'][:time])))
        print('the number of call drops: {}'.format(np.sum(macro_cell_stat['dropped_calls'][:time]))),
        print('the number of blocks due to capacity: {}'.format(np.sum(macro_cell_stat['num_cap_blocks'][:time])))
        print('the number of blocks due to signal strength: {}'.format(np.sum(macro_cell_stat['num_sig_blocks'][:time])))


def calculate_rsl(user, basic_param, base_param, user_param, shadow):
    """compute rsl of mobile from macro and small cell
    """
    macro_pl = prop_loss(user, basic_param, base_param, user_param, base_param.macro_f) - shadowing_loss(user, shadow, user_param) - fading_loss()
    small_pl = prop_loss(user, basic_param, base_param, user_param, base_param.small_f) - shadowing_loss(user, shadow, user_param) - fading_loss()    
    
    macro_rsl = base_param.macro_EIRP - macro_pl
    small_rsl = base_param.small_EIRP - small_pl
    
    return {'macro': macro_rsl, 'small': small_rsl}


def prop_loss(user, basic_param, base_param, user_param, freq):
    """compute propagation loss
    """
        
    # convert into km for calculation
    d1_km = basic_param.base_d/1000
    d2_km = user.loc/1000
    
    # calculate distance from cell to user
    if user.is_road == True:
        d = np.sqrt(np.square(d1_km) + np.square(d2_km))
        
    else:
        d = d2_km
        
    a = (1.1 * np.log10(freq) - 0.7) * user_param.mobile_h - (1.56 * np.log10(freq) - 0.8)
    b = (44.9 - 6.55 * np.log10(base_param.base_h)) * np.log10(d) - 13.82 * np.log10(base_param.base_h) - a
        
    if freq >= 1500: # COST-231
        c_m = 0
        return 46.3 + 33.9 * np.log10(freq) + b + c_m
    
    else: # Okamura_Hata
        return 69.55 + 26.16 * np.log10(freq) + b


def shadowing_loss(user, shadow, user_param):
    """compute shadowing loss
    """
    
    if user.is_road == True:
        value = int((user.loc + 3500)//user_param.shadowing_res)
        idx = np.clip(value, 0, 1400)
        
        return shadow[idx]
    
    else:
        return 2


def fading_loss():
    """compute fading loss
    """
    
    # convert into decibels
    rayleigh_samp = 20 * np.log10(np.random.rayleigh(size=10))
    
    # sort samples
    rayleigh_samp.sort()
    
    return rayleigh_samp[1] # return the 2nd deepest fading value among 10 values