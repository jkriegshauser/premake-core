--
-- tests/test_lua.lua
-- Automated test suite for Lua base functions.
-- Copyright (c) 2008 Jess Perkins and the Premake project
--

	local suite = test.declare("lua")

--
-- loadfile
--

local real_io_open = io.open

function suite.loadfile()
	local file =  path.join(_SCRIPT_DIR, "test_lua_loaded_noenv.lua")
	local fn = assert(loadfile(file, nil))
	local ret, value = pcall(fn)
	test.isequal(10, value)
end




--
-- loadfile with custom env
--

	function suite.loadfile_with_env()
		local file =  path.join(_SCRIPT_DIR, "test_lua_loaded.lua")
		local value = 0
		local env = {
			["foobar"] = function(n) value = n end
		}
		local fn = assert(loadfile(file, nil, env))
		pcall(fn)
		test.isequal(10, value)
	end


--
-- loadfile via require with init.lua
--

function suite.loadfile_with_require()
	os.chdir(_SCRIPT_DIR)
	assert(require("folder"))
end


--
-- loadfile with Unicode path
--

function suite.loadfile_UnicodeFilename()
	local p = os.tmpname()
	os.remove(p)
	p = p .. "_café.lua"
	local f = assert(real_io_open(p, "w"))
	f:write("return 42\n")
	f:close()
	local fn = assert(loadfile(p))
	local ok, value = pcall(fn)
	os.remove(p)
	test.istrue(ok)
	test.isequal(42, value)
end


--
-- io.open with Unicode filenames
--

function suite.io_open_WriteReadUnicodeFilename()
	local p = os.tmpname()
	os.remove(p)
	p = p .. "_café"
	local f = assert(real_io_open(p, "w"))
	f:write("hello unicode")
	f:close()
	f = assert(real_io_open(p, "r"))
	local content = f:read("*a")
	f:close()
	os.remove(p)
	test.isequal("hello unicode", content)
end

function suite.io_open_AppendModeUnicodeFilename()
	local p = os.tmpname()
	os.remove(p)
	p = p .. "_données"
	local f = assert(real_io_open(p, "w"))
	f:write("first")
	f:close()
	f = assert(real_io_open(p, "a"))
	f:write("second")
	f:close()
	f = assert(real_io_open(p, "r"))
	local content = f:read("*a")
	f:close()
	os.remove(p)
	test.isequal("firstsecond", content)
end

function suite.io_open_ReturnsNil_OnMissingUnicodeFile()
	local f, err = real_io_open(os.tmpname() .. "_noëxist", "r")
	test.isnil(f)
	test.isequal("string", type(err))
end


--
-- io.popen
--

function suite.io_popen_ReadsOutput()
	local f = io.popen("echo hello", "r")
	test.istrue(f ~= nil)
	if f then
		local content = f:read("*l")
		f:close()
		test.istrue(content ~= nil and content:find("hello") ~= nil)
	end
end

