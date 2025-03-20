#rm img/img_*.pbm
ffmpeg -r 30 -i img/img_%d.pbm -vcodec libx264 -crf 25  -pix_fmt yuv420p movi_game_of_life.mp4
