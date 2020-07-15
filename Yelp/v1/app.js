var express = require('express');
var app = express();

app.set('view engine', "ejs");

app.get('/', function(req, res){
    res.send("hello world");
})

app.get('/campgrounds');

// http://localhost:process.env.PORT
app.listen(3000, process.env.IP, function(){
    console.log("YelpCamp Has Started!");
});