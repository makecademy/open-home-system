<?php

  // Respond to GET requests
  if ($_GET) {

    // Initialization phases
    if ($_GET["device"] && $_GET["phase"]) {

      // Normal operation phase
      if ($_GET["phase"] == "operation") {

        $central = simplexml_load_file('central.xml');

        $central->$_GET["device"]->Registered = "True";
        $central->$_GET["device"]->addChild("Value", "Off"); 

        $central->asXML('central.xml');

      }

      // For relay
      if ($_GET["phase"] == "switch") {

        $central = simplexml_load_file('central.xml');
        $current_state = $central->$_GET["device"]->Value;

        if ($current_state == "Off") {
          $central->$_GET["device"]->Value = "On";
        }

        else {
          $central->$_GET["device"]->Value = "Off";
        }

        $central->asXML('central.xml');
      }

    }

  }

?>