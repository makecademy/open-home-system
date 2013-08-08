<?php
	
	// Load XML file
	$central = simplexml_load_file('central.xml');
	echo "<div class='device_title'>Device name</div>";
	// Echo XML file, parse devices
	foreach($central->children() as $child) {
		echo "<div class='device'>";
  		echo $child->getName()." ";
  		
  		// Parse attributes of devices
  		foreach($child->children() as $subchild) {
  			if ($subchild->getName() == "Registered"){
  				
  			}

  			if ($subchild->getName() == "Value"){
  				if($subchild == "Off") {
  					echo "<div class='off'></div>";
  				}

  				if($subchild == "On") {
  					echo "<div class='on'></div>";
  				}

  			}

  			// If a device is not registered, propose to register
    		if ($subchild->getName() == "Registered" && $subchild == "False"){
    			echo "<input id='register' value='Register device' type='submit'>
    			<script src='jquery-2.0.3.min.js'></script>
					<script type='text/javascript'>
           				$('#register').click(function(){
                 			$.get('central.php',{device:'".$child->getName()."',phase:'operation'});
            			});
    				</script>";
    		}
  		}
  		echo "</div>";
	}

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

				$device = $central->addchild($_GET["device"]);
				$device->addChild("Registered", "False"); 

				$central->asXML('central.xml');

				echo "Device detected";

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

			// Normal operation phase
			if ($_GET["phase"] == "operation") {

				$central = simplexml_load_file('central.xml');

				$central->$_GET["device"]->Registered = "True";
				$central->$_GET["device"]->addChild("Value", "Off"); 

				$central->asXML('central.xml');

			}

		}

	}

?>