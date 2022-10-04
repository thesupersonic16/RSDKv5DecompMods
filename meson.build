project('RSDKv5SteamAPI', ['c', 'cpp'],
  meson_version: '>= 0.59.0',
  default_options : ['cpp_std=c++17']
)

mod = meson.project_name()
src = meson.source_root()

sources = [
    'GameAPI/CPP/GameAPI/Game.cpp',
    'subhook/subhook.c',
    f'@mod@/dllmain.cpp',
    f'@mod@/Mod.cpp',
]

includes = [
    'GameAPI/CPP/GameAPI/RSDKv5',
    'GameAPI/CPP/GameAPI/',
    'subhook',
    mod,
    f'@mod@/include/steam',
]

add_global_link_arguments(f'@src@/@mod@/lib/libsteam_api.so', '-Wl,-rpath=$ORIGIN', language: ['c', 'cpp'])

install_data(f'@mod@/mod.ini', install_dir : f'./mods/@mod@')

library(mod, sources, include_directories : includes, install_dir : './mods/@mod@')
