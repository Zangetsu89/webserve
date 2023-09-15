<?php

// This works only for Chrome!
// get all data from STDIN

$postData = '';
while ($line = fgets(STDIN)) {
    $postData .= $line;
}

// print_r("<!DOCTYPE html><html><header><title>PHP output tes</title></header><body>");

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

print_r("boundary is ".$boundary);

foreach ($_POST as $key => $value) 
{
	if (substr($value, -3) === "\n--") {
	    $value = substr($value, 0, -3);
	} else {
	    $value = $value;
	}
	print_r("!!".$key."!! = !!".$value."!!<br>" );
}

print_r("Uploaded File name is ");
print_r($_FILES["file"]["name"]."<br>");
print_r("REQUEST_METHOD is ");
print_r($_ENV['REQUEST_METHOD']."<br>");
print_r("SERVER_PROTOCOL is ");
print_r($_ENV['SERVER_PROTOCOL']."<br>");
print_r("CONTENT_LENGTH is ");
print_r($_ENV['CONTENT_LENGTH']."<br>");
print_r("CONTENT_TYPE is ");
print_r($_ENV['CONTENT_TYPE']."<br>");
print_r("QUERY_STRING is ");
print_r($_ENV['QUERY_STRING']."<br>");
print_r("SCRIPT_FILENAME is ");
print_r($_ENV['SCRIPT_FILENAME']."<br>");
print_r("SCRIPT_NAME is ");
print_r($_ENV['SCRIPT_NAME']."<br>!!");



print_r("hahaha");

?>
