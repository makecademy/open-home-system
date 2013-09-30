<?php

//Reduce errors
error_reporting(~E_WARNING);
 
//Create a UDP socket
if(!($sock = socket_create(AF_INET, SOCK_DGRAM, 0)))
{
    $errorcode = socket_last_error();
    $errormsg = socket_strerror($errorcode);
     
    die("Couldn't create socket: [$errorcode] $errormsg \n");
}
 
echo "Socket created \n";
 
// Bind the source address
if( !socket_bind($sock, "192.168.0.1" , 9999) )
{
    $errorcode = socket_last_error();
    $errormsg = socket_strerror($errorcode);
     
    die("Could not bind socket : [$errorcode] $errormsg \n");
}
 
echo "Socket bind OK \n";
 
//Do some communication, this loop can handle multiple clients
while(1)
{
    echo "Waiting for data ... \n";

    $answer = "OK";
     
    //Receive some data
    $r = socket_recvfrom($sock, $buf, 512, 0, $remote_ip, $remote_port);
    echo "Message received: " . $buf;
    echo "\n";

    parse_str($buf);

    // Normal operation
	if (isset($device) && isset($phase)) {

		echo "Device: " . $device . "\n";
    	echo "Phase: " . $phase . "\n";
    	
		// Detection phase
		if ($phase == "detection") {

			$central = simplexml_load_file('central.xml');

			if (isset($central->$device->Registered)) {
				$answer = "Device detected";
			}

			else {

				$module = $central->addchild($device);
				$module->addChild("Registered", "False");
				$module->addChild("Type", preg_replace("/[^a-zA-Z]+/", "", $device));
				$central->asXML('central.xml');
				$answer = "Device detected";

			}

		}

		// Registration phase
		if ($phase == "registration") {

			$central = simplexml_load_file('central.xml');

			if ($central->$device->Registered == "True") {
				$answer = "Device registered";
			}
			else {
				$answer = "Not yet registered";
			}

		}

		if ($phase == "command") {

				$central = simplexml_load_file('central.xml');
				$answer = $central->$device->Value . " ";

		}

		if ($phase == "receive") {

				$central = simplexml_load_file('central.xml');
			    $central->$device->Value = $data;
			    $central->asXML('central.xml');
			    $answer = "Data received";
		}

	}

    // Answer
	echo "Answer: " . $answer . "\n" . "\n";
    socket_sendto($sock, $answer , 512 , 0 , $remote_ip , $remote_port);
}
 
socket_close($sock);

?>