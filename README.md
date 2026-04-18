# Color convertor project is a project which help convert raw data between pixel formats

# Support
  ### Pixel format
  - rgb24
  - yuv420p, nv12, nv21, yuv422p, yuv444p
  ### Color space
  - bt601
  - bt709
  - bt2020
  #### Color range
  - limited range
  - full range

# User guide
  - clone the project
  - compile by command: "g++ -std=c++11 -O2 src/main.cpp src/color_convertor.cpp src/utils.cpp src/file_loader.cpp -o color_convertor2"
  - test by command: "./color_convertor2 input_file output_file width height origin_fmt next_fmt color_space color_range"
  - view by ffplay command: "ffplay -f rawvideo -video_size widthxheight -pixel_format color_fmt -i input_file"

# Required
  - c++ programming
  - g++ complier
  - ffmpeg
