<!DOCTYPE html>
<html xmlns="http://www.w3.org/1999/xhtml" lang="en">
<head>
<title>save and delete</title>
<link rel="stylesheet" type="text/css" href="../../css/common.css" media="screen">
</head>
<body>
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

// Parse the CONTENT_TYPE to get the boundary
$boundary = str_replace('boundary=', '', $_ENV['CONTENT_TYPE']);
$boundarys = explode(' ', $boundary);


// Split the data into individual fields
$fields = explode($boundarys[1], $postData);

// Process each field
foreach ($fields as $field) {
	if (empty($field)) continue;

	// Separate the field headers and value
	list($headers, $value) = explode("\r\n\r\n", $field, 2);

	// Parse the headers to get the field name (and possibly filename)
	// Handle the field value (e.g., store it in $_POST)
	if (preg_match('/name="([^"]+)"/', $headers, $matches))
	{
		$fieldName = $matches[1];		
		$_POST[$fieldName] = $value;
	}
	if (preg_match('/filename="([^"]+)"/', $headers, $matches))
	{
		$fileName = $matches[1];		
		$_POST["posted_filename"] = $fileName;
	}
}


foreach ($_POST as $key => $value) 
{
	if (substr($value, -3) === "\n--") {
	    $value = substr($value, 0, -3);
	} else {
	    $value = $value;
	}
	if (substr($value, -4) === "\r\n--") {
	    $value = substr($value, 0, -4);
	} else {
	    $value = $value;
	}
	// print_r("!!".$key."!! = !!".$value."!!<br>" );
}

if (substr($_POST['uploading_'], -2) === "--") {
	$_POST['uploading_'] = substr($_POST['uploading_'], 0, -2);
}


$input_data = $_POST['uploading_'];
// print_r($_POST['uploading_']."<br>");


// echo "<br>HAHAHA<br>";
// print_r($_POST['posted_filename']."<br>");

// echo "HAHAHA<br>";

// print_r("Uploaded File name is ");
// print_r($_FILES["file"]["name"]."<br>");
// print_r("REQUEST_METHOD is ");
// print_r($_ENV['REQUEST_METHOD']."<br>");
// print_r("SERVER_PROTOCOL is ");
// print_r($_ENV['SERVER_PROTOCOL']."<br>");
// print_r("CONTENT_LENGTH is ");
// print_r($_ENV['CONTENT_LENGTH']."<br>");
// print_r("CONTENT_TYPE is ");
// print_r($_ENV['CONTENT_TYPE']."<br>");
// print_r("QUERY_STRING is ");
// print_r($_ENV['QUERY_STRING']."<br>");
// print_r("SCRIPT_FILENAME is ");
// print_r($_ENV['SCRIPT_FILENAME']."<br>");
// print_r("SCRIPT_NAME is ");
// print_r($_ENV['SCRIPT_NAME']."<br>");
// print_r("SERVER_NAME is ");
// print_r($_ENV['SERVER_NAME']."<br>");


$uploadpath = __DIR__."/../upload/";

print_r("Hello ".$_POST['first_name']." ".$_POST['last_name']);

if ($_POST['posted_filename'])
{
	$uploadfilepath = $uploadpath.$_POST['posted_filename'];
	// print_r($uploadfilepath."<br>");
	$myfile = fopen($uploadfilepath, "w");
	if ($myfile > 0)
	{
		fwrite($myfile, $_POST['uploading_']);
		fclose($myfile);
	}
	echo "<H1>File save DONE!!!</H1>";
	echo "<p>File saved.</p><br>";
	echo "<h3>Do you want to see?<br></h3>";
	echo "<a href=\"../upload/".$_POST['posted_filename']."\" target=\"_blank\">".$_POST["posted_filename"]."</a><br>";

	echo "<h3>Do you want to delete?<br></h3>";
	echo "<form action=\"delete.php\" method=\"POST\">";
	// echo "<label for=\"file_path\">File Path:</label>";
	echo "<input type=\"hidden\" value=\"".$uploadfilepath."\" name=\"deletefile\">";
	echo "<input type=\"submit\" value=\"delete\" class=\"send\"></form>";

}
else
{
	echo "file is not uploaded (already exist)";
}

?>

</body>
</html>
