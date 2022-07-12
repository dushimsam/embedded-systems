import requests
import json

api_url = "http://insecure.benax.rw/iot/"

req ={"device":"dusham","distance":"25cm"}
headers =  {"Content-Type":"application/json"}

response = requests.post(api_url,req)
# response.raise_for_status()  # raises exception when not a 2xx response

# if response.status_code != 204:
print(response.text)
# response = requests.get(api_url)
