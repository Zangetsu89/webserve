import sys

success = sys.argv[1:][0]
print("Content-Type: text/plain")
if success == "true":
	print("Data saved successfully.")
else
	print("Data cannot be saved.")