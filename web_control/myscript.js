$.ajaxSetup({timeout:100});

var latch = false;

function readUrlAV (form) {
    TextVar = form.inputbox.value;
	VideoVar = "http://"+TextVar+":8080/video";
	AudioVar = "http://"+TextVar+":8080/audio.opus";
	document.getElementById("video").setAttribute('data', VideoVar);
	document.getElementById("audio").setAttribute('data', AudioVar);
}
function testarArduino (form) {
    TextVar = myform2.inputbox.value;
	ArduinoVar = "http://" + TextVar + ":80";
	$.get( ArduinoVar, { "cm3": 7000 })	;
	{Connection: close};
}

function setRPM (form) {
    TextVar = myform2.inputbox.value;
    ArduinoVar = "http://" + TextVar + ":80";
    formRpmVar = formRPM.inputbox.value;
    rpmSet = "a" + formRpmVar;
    $.get( ArduinoVar, {number: rpmSet});
	{Connection: close};
}


document.onkeydown = checkKeyDown;
document.onkeyup = checkKeyUp;

function checkKeyDown(e) {

    e = e || window.event;

    if (e.keyCode == '38') {
        // up arrow
		if (latch == false) {
			TextVar = myform2.inputbox.value;
			ArduinoVar = "http://" + TextVar + ":80";
			$.get( ArduinoVar, { "01": 1000 })	;
			{Connection: close};
			latch = true;
		}
    }
    else if (e.keyCode == '40') {
        // down arrow
		if (latch == false) {
			TextVar = myform2.inputbox.value;
			ArduinoVar = "http://" + TextVar + ":80";
			$.get( ArduinoVar, { "02": 1000 })	;
			{Connection: close};
			latch = true;
		}
    }
    else if (e.keyCode == '37') {
       // left arrow
	   if (latch == false) {
			TextVar = myform2.inputbox.value;
			ArduinoVar = "http://" + TextVar + ":80";
			$.get( ArduinoVar, { "05": 1000 })	;
			{Connection: close};
			latch = true;
		}

	}
    else if (e.keyCode == '39') {
       // right arrow
	   if (latch == false) {
			TextVar = myform2.inputbox.value;
			ArduinoVar = "http://" + TextVar + ":80";
			$.get( ArduinoVar, { "04": 1000 })	;
			{Connection: close};
			latch = true;
		}
    }

    // q: pivot right
    else if (e.keyCode == '81') {
	   if (latch == false) {
			TextVar = myform2.inputbox.value;
			ArduinoVar = "http://" + TextVar + ":80";
			$.get( ArduinoVar, { "13": 1000 })	;
			{Connection: close};
			latch = true;
		}
    }
    // w: pivot left
    else if (e.keyCode == '87') {
	   if (latch == false) {
			TextVar = myform2.inputbox.value;
			ArduinoVar = "http://" + TextVar + ":80";
			$.get( ArduinoVar, { "14": 1000 })	;
			{Connection: close};
			latch = true;
		}
    }

    // i: increase speed 
    else if (e.keyCode == '73') {
       // i character
	   if (latch == false) {
			TextVar = myform2.inputbox.value;
			ArduinoVar = "http://" + TextVar + ":80";
			$.get( ArduinoVar, { "06": 1000 })	;
			{Connection: close};
			latch = true;
		}
    }
    // d: decrease speed
    else if (e.keyCode == '68') {
       // d character
	   if (latch == false) {
			TextVar = myform2.inputbox.value;
			ArduinoVar = "http://" + TextVar + ":80";
			$.get( ArduinoVar, { "07": 1000 })	;
			{Connection: close};
			latch = true;
		}
    }

    // r: reset speed/microstep
    else if (e.keyCode == '82') {
       // r character
	   if (latch == false) {
			TextVar = myform2.inputbox.value;
			ArduinoVar = "http://" + TextVar + ":80";
			$.get( ArduinoVar, { "08": 1000 })	;
			{Connection: close};
			latch = true;
		}
	}
    // 1: full speed/microstep
    else if (e.keyCode == '49') {
       // 1 character
	   if (latch == false) {
			TextVar = myform2.inputbox.value;
			ArduinoVar = "http://" + TextVar + ":80";
			$.get( ArduinoVar, { "09": 1000 })	;
			{Connection: close};
			latch = true;
		}
    }    // 2: 1/2 speed/microstep
    else if (e.keyCode == '50') {
       // 2 character
	   if (latch == false) {
			TextVar = myform2.inputbox.value;
			ArduinoVar = "http://" + TextVar + ":80";
			$.get( ArduinoVar, { "10": 1000 })	;
			{Connection: close};
			latch = true;
		}
    }    // 4: 1/4 speed/microstep
    else if (e.keyCode == '52') {
       // r character
	   if (latch == false) {
			TextVar = myform2.inputbox.value;
			ArduinoVar = "http://" + TextVar + ":80";
			$.get( ArduinoVar, { "11": 1000 })	;
			{Connection: close};
			latch = true;
		}
    }    // 8: 1/8 speed/microstep
    else if (e.keyCode == '56') {
       // r character
	   if (latch == false) {
			TextVar = myform2.inputbox.value;
			ArduinoVar = "http://" + TextVar + ":80";
			$.get( ArduinoVar, { "12": 1000 })	;
			{Connection: close};
			latch = true;
		}
	}

}

function checkKeyUp(e) {
    e = e || window.event;

    if ((e.keyCode == '38')||(e.keyCode == '40')||(e.keyCode == '37')||(e.keyCode == '39') || (e.keyCode == '56') || (e.keyCode == '52')
        || (e.keyCode == '50') || (e.keyCode == '49') || (e.keyCode == '82') || (e.keyCode == '68') || (e.keyCode =='73') || (e.keyCode =='87') || (e.keyCode == '81') ){ 

		setTimeout(doNothing, 200);
    }
}

function doNothing(){
	TextVar = myform2.inputbox.value;
		ArduinoVar = "http://" + TextVar + ":80";
		$.get( ArduinoVar, { "03": 1000 })	;
		{Connection: close};
		latch = false;
}
