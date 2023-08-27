import sys
import mimetypes

#if type is not included in the list, it should be an attachment as application/octet-stream

file_path = sys.argv[1:][0]

try:
    with open(file_path, 'rb') as file:
        file_content = file.read()

    # Set the appropriate Content-Type based on the file content
    mime_type, _ = mimetypes.guess_type(file_path)
    if mime_type:
        content_type = mime_type;
    else:
        content_type = application/octet-stream

    # Get the filename from the file path
    filename = file_path.split('/')[-1]

    print("Content-type: " + content_type+"\n")
    if content_type == "application/octet-stream":
        print("Content-Disposition: attachment; filename=" + filename+ "\n")
    
    sys.stdout.flush()  # Flush any previous output

    # Output the file content
    sys.stdout.buffer.write(file_content)

except FileNotFoundError:
    print("Status: 404 Not Found")
    print("Content-type: text/html\n")
    print("<html><body><h1>File Not Found</h1></body></html>")