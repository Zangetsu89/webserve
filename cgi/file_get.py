import sys

file_path = sys.argv[1:][0]

try:
    with open(file_path, 'rb') as file:
        file_content = file.read()

    # Get the filename from the file path
    filename = file_path.split('/')[-1]

    # Set the appropriate Content-Type based on the file extension
    if filename.endswith('.txt'):
        content_type = 'text/plain'
    else:
        content_type = 'application/octet-stream'

    print(f"Content-type: {content_type}")
    print(f"Content-Disposition: attachment; filename={filename}\n")
    
    sys.stdout.flush()  # Flush any previous output

    # Output the file content
    sys.stdout.buffer.write(file_content)

except FileNotFoundError:
    print("Status: 404 Not Found")
    print("Content-type: text/html\n")
    print("<html><body><h1>File Not Found</h1></body></html>")