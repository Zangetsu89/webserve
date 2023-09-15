<!DOCTYPE html>
<html xmlns="http://www.w3.org/1999/xhtml" lang="en">
<head>
<title>delete done</title>
<link rel="stylesheet" type="text/css" href="../../css/common.css" media="screen">
</head>

<?php

// This works only for Chrome!
// get all data from STDIN

$postData = '';
while ($line = fgets(STDIN)) {
    $postData .= $line;
}

// print_r("<br><p><H1>---Got all data!---</H1></p><br><br>");
// print_r($postData);
// print_r("<p>---Data end!---</p><br><br>");

$fields = explode("=", $postData);


// print_r($fields[0]." is ".$fields[1]);

$decodedText = urldecode($fields[1]);
// echo $decodedText; 
if (unlink($decodedText))
{
	echo "<H3>File is deleted!!!</H3><br>";
}

?>
<a href="../../upload_php.html">upload again?</a>
</body>
</html>
