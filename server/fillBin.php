<?php
  require 'DBlogin.php';

    if (empty($_GET["id"])) {
      echo "please provide id";
    }elseif (empty($_GET["fill"])) {
      echo "please provide fill";
    }elseif (empty($_GET["battery"])) {
        echo "please provide battery";
    }elseif (empty($_GET["pw"])) {
      echo "please provide a password";
    }elseif ($_GET["pw"]!="thorSmartBin") {
      echo "wrong password provided";
    } else {
      $fill=$_GET["fill"];
      $battery=$_GET["battery"];
      $id=$_GET["id"];
      $conn = new mysqli($servername, $username, $password, $dbname);

      if ($conn->connect_error) {
        echo "connection error";
        die("Connection failed: " . $conn->connect_error);
      }
      $sql = "UPDATE bins SET volume='".$fill."', battery='".$battery."' WHERE id='".$id."'";
      if($result = $conn->query($sql)) {
                echo "dont restart";
            } else {
                echo "error";
            }

    }

?>
