<?php
	
	// Load XML file
	$central = simplexml_load_file('central.xml');
	echo "<div class='device_title'>Home</div>";
	// Echo XML file, parse devices
	foreach($central->children() as $child) {
		echo "<div class='device'>";
  		echo $child->getName()." ";
  		
  		// Parse attributes of devices
  		foreach($child->children() as $subchild) {
  			if ($subchild->getName() == "Registered"){
  				
  			}

  			if ($subchild->getName() == "Type"){
  				$type = $subchild;
  			}

  			if ($subchild->getName() == "Value"){

          if ($type == "temp"){
              echo "<div class='temp'>" . $subchild . "°C" . "</div>";
          }


  				if ($type == "motion" || $type == "contact"){
  					if($subchild == "Off") {
  						echo "<div class='off'></div>";
  					}

  					if($subchild == "On") {
  						echo "<div class='on'></div>";
  					}
  				}

  				if ($type == "led"){
  					if($subchild == "Off") {
  						echo "<div class='onoffswitch'>
            <input type='checkbox' name='onoffswitch' class='onoffswitch-checkbox' id='myonoffswitch'>
            <label class='onoffswitch-label' for='myonoffswitch'>
            <div class='onoffswitch-inner'></div>
            <div class='onoffswitch-switch'></div>
            </label>
            </div>
						<script type='text/javascript'>
           				$('#myonoffswitch').click(function(){
                 			$.get('interface.php',{device:'".$child->getName()."',phase:'switch'});
            			});
    					</script>";
  					}

  					if($subchild == "On") {
  						echo "<div class='onoffswitch'>
            <input type='checkbox' name='onoffswitch' class='onoffswitch-checkbox' id='myonoffswitch' checked>
            <label class='onoffswitch-label' for='myonoffswitch'>
            <div class='onoffswitch-inner'></div>
            <div class='onoffswitch-switch'></div>
            </label>
            </div>
						<script type='text/javascript'>
           				$('#myonoffswitch').click(function(){
                 			$.get('interface.php',{device:'".$child->getName()."',phase:'switch'});
            			});
    					</script>";
  					}
  				}
  				
  			}

  			// If a device is not registered, propose to register
    		if ($subchild->getName() == "Registered" && $subchild == "False"){
    			echo "<input id='register' value='Register device' type='submit'>
    			<script src='jquery-2.0.3.min.js'></script>
					<script type='text/javascript'>
           				$('#register').click(function(){
                 			$.get('interface.php',{device:'".$child->getName()."',phase:'operation'});
            			});
    				</script>";
    		}
  		}
  		echo "</div>";
	}

?>