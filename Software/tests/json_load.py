import json
from pprint import pprint

loc = "D:\Projects\Halo Controller\Settings.json"

with open(loc) as settings_file:
    settings = json.load(settings_file)

pprint(settings)

if settings["pages"]["flight_stats"]["active"]:
    print("Working!")
