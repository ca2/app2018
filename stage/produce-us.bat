@echo going to telmico/votagus production folder
T:
cd \production\stage\x64\
ca2app.exe : app=production start_deferred=1 base_dir=T:\stage version=stage