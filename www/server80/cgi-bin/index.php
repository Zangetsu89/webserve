<?php

//  phpinfo();

$say = htmlspecialchars($_POST['say']);
$to  = htmlspecialchars($_POST['to']);

echo  $_GET['say'], ' ', $to;
echo " DONE!\n";


?>