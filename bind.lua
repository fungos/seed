require 'lubyk'

local base = lk.directory(lk.file())

local ins = dub.Inspector {
  INPUT    = {
    base .. '/include/',
  },
  doc_dir = base .. '/bind/tmp',
}

local binder = dub.LuaBinder()

binder:bind(ins, {
  output_directory = base .. '/bind/lua',
  header_base = base .. '/include',
  single_lib = 'seed',
  luaopen    = 'seed_core',
  only = only,
})
