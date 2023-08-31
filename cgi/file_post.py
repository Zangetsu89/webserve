import sys

path = sys.argv[1:][0]
content_type = sys.argv[1:][1]
content = sys.argv[1:][2]

def save_file(file_path, content):
    try:
        with open(file_path, 'w') as file:
            file.write("Content-type: " content_type + content)
        success = "true"
    except Exception as e:
        success = "false"

save_file(content_type, path, content)

print("Content-Type: text/plain")
if success == "true":
	print("<html>Data saved successfully.")
else
	print("Data cannot be saved.")