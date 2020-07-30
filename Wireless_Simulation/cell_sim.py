#!/usr/bin/env python
# coding: utf-8

import random
import time
from collections import namedtuple
import matplotlib.pyplot as plt
import numpy as np
import cell # please put cell.py in the same directory with the script file

if __name__ == "__main__":
    # Simulation 1
    Basic_param = namedtuple('Basic_params', ['road_len', 'base_d', 'enter_thresh', 'shop_rad', 'shop_time_mu', 'shop_time_sigma', 'time_step', 'total_time', 'p'])
    basic_param = Basic_param(7000, 1000, 20, 900, 1800, 200, 1, (3600*4)+1, 0.25)

    Base_param = namedtuple('Base_params', ['base_h', 'macro_EIRP', 'small_EIRP', 'macro_num_channels', 'small_num_channels','macro_f','small_f'])
    base_param = Base_param(45, 57, 54, 35, 35, 800, 1900)

    User_param = namedtuple('User_params', ['mobile_h', 'rx_thresh', 'small_thresh', 'HO_margin', 'num_users', 'call_rate', 'call_dur', 'mobile_speed', 'shadowing_res'])
    user_param = User_param(1.7, -102, -80, 3, 1000, 1/3600, 180, 33, 5)

    # Your program should also produce a summary report at the end for the entire simulation time
    macro_cell_stat = {'num_channels': np.zeros(basic_param.total_time), 
                        'call_attempts': np.zeros(basic_param.total_time), 
                        'success_connections': np.zeros(basic_param.total_time),
                        'completed_calls': np.zeros(basic_param.total_time), 
                        'HO_attempt_m2s': np.zeros(basic_param.total_time),
                        'HO_success_m2s': np.zeros(basic_param.total_time),
                        'HO_failure_m2s': np.zeros(basic_param.total_time),
                        'dropped_calls': np.zeros(basic_param.total_time),
                        'num_cap_blocks': np.zeros(basic_param.total_time),
                        'num_sig_blocks': np.zeros(basic_param.total_time)}

    small_cell_stat = {'num_channels':np.zeros(basic_param.total_time),
                        'call_attempts':np.zeros(basic_param.total_time), 
                        'success_connections':np.zeros(basic_param.total_time),
                        'completed_calls':np.zeros(basic_param.total_time),
                        'HO_attempt_s2m':np.zeros(basic_param.total_time),
                        'HO_success_s2m':np.zeros(basic_param.total_time),
                        'HO_failure_s2m':np.zeros(basic_param.total_time),
                        'dropped_calls':np.zeros(basic_param.total_time),
                        'num_cap_blocks':np.zeros(basic_param.total_time),
                        'num_sig_blocks':np.zeros(basic_param.total_time)}

    # run simulation 5 times
    for _ in range(5):
        macro_cell_stat, small_cell_stat = cell.run_sim(basic_param, base_param, user_param, macro_cell_stat, small_cell_stat)



    # Simulation 2
    Basic_param = namedtuple('Basic_params', ['road_len', 'base_d', 'enter_thresh', 'shop_rad', 'shop_time_mu', 'shop_time_sigma', 'time_step', 'total_time', 'p'])
    basic_param = Basic_param(7000, 1000, 20, 900, 1800, 200, 1, (3600*4)+1, 0.5)

    Base_param = namedtuple('Base_params', ['base_h', 'macro_EIRP', 'small_EIRP', 'macro_num_channels', 'small_num_channels','macro_f','small_f'])
    base_param = Base_param(45, 57, 54, 35, 35, 800, 1900)

    User_param = namedtuple('User_params', ['mobile_h', 'rx_thresh', 'small_thresh', 'HO_margin', 'num_users', 'call_rate', 'call_dur', 'mobile_speed', 'shadowing_res'])
    user_param = User_param(1.7, -102, -80, 3, 1000, 1/3600, 180, 33, 5)

    # Your program should also produce a summary report at the end for the entire simulation time
    macro_cell_stat2 = {'num_channels': np.zeros(basic_param.total_time), 
                        'call_attempts': np.zeros(basic_param.total_time), 
                        'success_connections': np.zeros(basic_param.total_time),
                        'completed_calls': np.zeros(basic_param.total_time), 
                        'HO_attempt_m2s': np.zeros(basic_param.total_time),
                        'HO_success_m2s': np.zeros(basic_param.total_time),
                        'HO_failure_m2s': np.zeros(basic_param.total_time),
                        'dropped_calls': np.zeros(basic_param.total_time),
                        'num_cap_blocks': np.zeros(basic_param.total_time),
                        'num_sig_blocks': np.zeros(basic_param.total_time)}

    small_cell_stat2 = {'num_channels':np.zeros(basic_param.total_time),
                        'call_attempts':np.zeros(basic_param.total_time), 
                        'success_connections':np.zeros(basic_param.total_time),
                        'completed_calls':np.zeros(basic_param.total_time),
                        'HO_attempt_s2m':np.zeros(basic_param.total_time),
                        'HO_success_s2m':np.zeros(basic_param.total_time),
                        'HO_failure_s2m':np.zeros(basic_param.total_time),
                        'dropped_calls':np.zeros(basic_param.total_time),
                        'num_cap_blocks':np.zeros(basic_param.total_time),
                        'num_sig_blocks':np.zeros(basic_param.total_time)}

    # run simulation 5 times
    for _ in range(5):
        macro_cell_stat2, small_cell_stat2 = cell.run_sim(basic_param, base_param, user_param, macro_cell_stat2, small_cell_stat2)



    # Simulation 3
    Basic_param = namedtuple('Basic_params', ['road_len', 'base_d', 'enter_thresh', 'shop_rad', 'shop_time_mu', 'shop_time_sigma', 'time_step', 'total_time', 'p'])
    basic_param = Basic_param(7000, 1000, 20, 900, 1800, 200, 1, (3600*4)+1, 0.25)

    Base_param = namedtuple('Base_params', ['base_h', 'macro_EIRP', 'small_EIRP', 'macro_num_channels', 'small_num_channels','macro_f','small_f'])
    base_param = Base_param(45, 57, 54, 35, 35, 800, 1900)

    User_param = namedtuple('User_params', ['mobile_h', 'rx_thresh', 'small_thresh', 'HO_margin', 'num_users', 'call_rate', 'call_dur', 'mobile_speed', 'shadowing_res'])
    user_param = User_param(1.7, -102, -80, 3, 2000, 1/1800, 180, 33, 5)

    # Your program should also produce a summary report at the end for the entire simulation time
    macro_cell_stat3 = {'num_channels': np.zeros(basic_param.total_time), 
                        'call_attempts': np.zeros(basic_param.total_time), 
                        'success_connections': np.zeros(basic_param.total_time),
                        'completed_calls': np.zeros(basic_param.total_time), 
                        'HO_attempt_m2s': np.zeros(basic_param.total_time),
                        'HO_success_m2s': np.zeros(basic_param.total_time),
                        'HO_failure_m2s': np.zeros(basic_param.total_time),
                        'dropped_calls': np.zeros(basic_param.total_time),
                        'num_cap_blocks': np.zeros(basic_param.total_time),
                        'num_sig_blocks': np.zeros(basic_param.total_time)}

    small_cell_stat3 = {'num_channels':np.zeros(basic_param.total_time),
                        'call_attempts':np.zeros(basic_param.total_time), 
                        'success_connections':np.zeros(basic_param.total_time),
                        'completed_calls':np.zeros(basic_param.total_time),
                        'HO_attempt_s2m':np.zeros(basic_param.total_time),
                        'HO_success_s2m':np.zeros(basic_param.total_time),
                        'HO_failure_s2m':np.zeros(basic_param.total_time),
                        'dropped_calls':np.zeros(basic_param.total_time),
                        'num_cap_blocks':np.zeros(basic_param.total_time),
                        'num_sig_blocks':np.zeros(basic_param.total_time)}

    macro_cell_stat3, small_cell_stat3 = cell.run_sim(basic_param, base_param, user_param, macro_cell_stat3, small_cell_stat3)