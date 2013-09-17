<?php

	// Respond to GET requests
	if ($_GET) {

		// Normal operation
		if ($_GET["device"] && $_GET["data"]) {

			$central = simplexml_load_file('central.xml');
    		$central->$_GET["device"]->Value = $_GET["data"];
    		$central->asXML('central.xml');

		}

		// Initialization phases
		if ($_GET["device"] && $_GET["phase"]) {

			// Detection phase
			if ($_GET["phase"] == "detection") {

				$central = simplexml_load_file('central.xml');

				if ($central->$_GET["device"]->Registered == "False") {
					echo "Device detected";
				}

				else {

					$device = $central->addchild($_GET["device"]);
					$device->addChild("Registered", "False");
					$device->addChild("Type", preg_replace("/[^a-zA-Z]+/", "", $_GET["device"])); 

					$central->asXML('central.xml');
					echo "Device detected";

				}

			}

			// Registration phase
			if ($_GET["phase"] == "registration") {

				$central = simplexml_load_file('central.xml');

				if ($central->$_GET["device"]->Registered == "True") {
					echo "Device registered";
				}
				else {
					echo "Not yet registered";
				}

			}

			if ($_GET["phase"] == "command") {

				$central = simplexml_load_file('central.xml');
				echo $central->$_GET["device"]->Value;
			}			

		}

	}

?>