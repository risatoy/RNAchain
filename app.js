const express = require('express');
const app = express();
var bodyParser = require("body-parser");

// tell express to serve the content of the public directory
app.use(express.static('public'));
app.use(bodyParser.urlencoded({extended: false}));
app.use(bodyParser.json());

app.get("/", (req, res) => {
	res.render("index.ejs", {stdout: ""});
})

app.post("/", (req, res) => {
	//* call c++ program
	// standard node module
	var execFile = require('child_process').execFile
	// pathe to executable
	var program = "./RNAchain";

	//from the browser
	var gEnzyme = req.body.genzyme;
	var ucEnzyme = req.body.ucenzyme;
	
	// this launches the executable and returns immediately
	var child = execFile(program,[gEnzyme, ucEnzyme], function (error, stdout, stderr) {
    	res.render("index.ejs", {stdout: stdout});
	});

})



app.listen(process.env.PORT || 3000);