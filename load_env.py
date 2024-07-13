# load_env.py
import os

def load_env_file(filepath):
    with open(filepath, 'r') as file:
        for line in file:
            if line.strip() and not line.startswith('#'):
                key, value = line.strip().split('=', 1)
                os.environ[key] = value

load_env_file('.env')

APPVERSION = os.getenv('APPVERSION')
APPUPDNAME = os.getenv('APPUPDNAME')
APPENCKEY = os.getenv('APPENCKEY')
APPDEVTYPE = os.getenv('APPDEVTYPE')

build_flags = f'-DAPPVERSION="{APPVERSION}" -DAPPUPDNAME="{APPUPDNAME}" -DAPPENCKEY="{APPENCKEY}" -DAPPDEVTYPE="{APPDEVTYPE}"'

print(build_flags)
