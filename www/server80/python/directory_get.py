import os
import sys

directory_path = sys.argv[1:][0]
permission = sys.argv[1:][1]

if (permission == "true"):
	def generate_directory_listing(directory_path):
		content = []

		# Get a list of files and subdirectories in the specified directory
		items = os.listdir(directory_path)

		for item in items:
			item_path = os.path.join(directory_path, item)
			if os.path.isdir(item_path):
				content.append(f'<li><strong>{item}/</strong></li>')
			else:
				content.append(f'<li>{item}</li>')

		return '\n'.join(content)

	print("Content-type: text/html\n")
	print("<html><head><title>Directory Listing</title></head>")
	print("<body>")
	print("<h1>Directory Listing</h1>")
	print("<ul>")
	listing = generate_directory_listing(directory_path)
	print(listing)
	print("</ul>")
	print("</body></html>")

else:
	print("Status: 403 Forbidden")
	print("Content-type: text/html\n")
	print("<html><head><title>403 Forbidden</title></head>")
	print("<body>")
	print("<h1>403 Forbidden</h1>")
	print("<p>You don't have permission to access this resource.</p>")
	print("</body></html>")