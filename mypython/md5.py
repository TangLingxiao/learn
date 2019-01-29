import hashlib
import os

m = hashlib.md5()
m.update('123456')
pw = m.hexdigest()
print pw
os.system('pause')