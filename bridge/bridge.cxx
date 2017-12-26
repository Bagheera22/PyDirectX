#include <Render.h>

#include <Python.h>
#include "CXX/Extensions.hxx"
#include "CXX/Objects.hxx"

#include <assert.h>
#include <exception>
#include <map>
#include <string>
#include <vector>
#include <sstream>

#ifdef _MSC_VER
// disable warning C4786: symbol greater than 255 character,
// nessesary to ignore as <map> causes lots of warning
#pragma warning(disable : 4786)
#endif

PYCXX_USER_EXCEPTION_STR_ARG(BadArguments)
namespace Helper
{
	std::string ToString(const std::vector<std::string>& values)
	{
		std::stringstream buffer;
		for (const auto& v : values)
		{
			buffer << v <<" ";
		}
		return buffer.str();
	}
}
class CheckAtributs
{
public:
	CheckAtributs(const Py::Tuple& args, const Py::Dict &kwds, const std::vector<std::string>& expected_keys)
	{
		if (args.size() == expected_keys.size())
		{
			for (std::size_t i = 0, l = args.size(); i < l; ++i)
			{
				m_values.insert(std::make_pair(expected_keys[i], args[i]));
			}
		}
		else if (kwds.size() == expected_keys.size())
		{
			const Py::List names(kwds.keys());
			for (Py::List::size_type i = 0, l = names.length(); i< l; i++)
			{
				Py::String name = names[i];
				Py::Object value = kwds.getItem(name);
				auto it =std::find(expected_keys.begin(), expected_keys.end(), name.as_string());
				if (it == expected_keys.end())
				{
					throw BadArguments("expected arguments: " + Helper::ToString(expected_keys)+ "  !=  "+ name.as_string());
				}
				m_values.insert(std::make_pair(name.as_string(), value));
			}

		}
		else
		{
			throw BadArguments("expected arguments: " + Helper::ToString(expected_keys) );
		}
	}
	Py::Object Get(const std::string& key)
	{
		const std::map<std::string, Py::Object>::iterator it = m_values.find(key);
		if (it != m_values.end())
		{
			return it->second;
		}
		return Py::None();
	}
private:
	std::map<std::string, Py::Object> m_values;
};
// Exceptions

class BridgeEngine : public Py::PythonClass<BridgeEngine>
{
public:
	BridgeEngine(Py::PythonClassInstance* self, Py::Tuple& args, Py::Dict& kwds)
		: Py::PythonClass<BridgeEngine>::PythonClass(self, args, kwds)
	{

	}

	virtual ~BridgeEngine()
	{
	}

	static void init_type(void)
	{
		behaviors().name("BridgeEngine");
		behaviors().doc("documentation for BridgeEngine class");
		behaviors().supportGetattro();
		behaviors().supportSetattro();
		PYCXX_ADD_KEYWORDS_METHOD(create_context, CreateContext, "docs for CreateContext");
		PYCXX_ADD_KEYWORDS_METHOD(render, Render, "docs for CreateContext");

		behaviors().readyType();
	}
	Py::Object CreateContext(const Py::Tuple& args, const Py::Dict &kwds)
	{
		CheckAtributs attr(args, kwds, { {"windowId","width", "height"} });

		const int width = static_cast<int>(Py::Long(attr.Get("width")).as_long());
		const int height = static_cast<int>(Py::Long(attr.Get("height")).as_long());
		PyObject * pyWinId = attr.Get("windowId").ptr();
		HWND hWnd = reinterpret_cast<HWND>(PyCapsule_GetPointer(pyWinId, NULL));
		printf("creating context: %d %d", width, height);
		bool ok = m_context.Create(hWnd, width, height);
		if (ok)
			std::cout << "si\n";
		else
			std::cout << "no\n";
		return Py::None();
	}

	PYCXX_KEYWORDS_METHOD_DECL(BridgeEngine, CreateContext)

	Py::Object Render(const Py::Tuple& /*args*/, const Py::Dict &/*kwds*/)
	{
		m_context.Render();
		return Py::None();
	}
	PYCXX_KEYWORDS_METHOD_DECL(BridgeEngine, Render)

private:
	Context m_context;
};

class BridgeEngine_module : public Py::ExtensionModule<BridgeEngine_module>
{
public:
	BridgeEngine_module()
		: Py::ExtensionModule<BridgeEngine_module>(
			  "BridgeEngine") // this must be name of the file on disk e.g. simple.so or simple.pyd
	{
		BridgeEngine::init_type();

		add_keyword_method("version", &BridgeEngine_module::version, "documentation for version()");

		// after initialize the moduleDictionary will exist
		initialize("documentation for the BridgeEngine_module module");

		Py::Dict d(moduleDictionary());
		d["var"] = Py::String("var value");

		d["BridgeEngine"] = Py::Object(BridgeEngine::type());

		BadArguments::init(*this);
	}

private:
	const char* VERSION = "0.0.0";
	Py::Object version(const Py::Tuple& args, const Py::Dict& /*kwds*/) { return Py::String(std::string(VERSION)); }
};

#if defined(_WIN32)
#define EXPORT_SYMBOL __declspec(dllexport)
#else
#define EXPORT_SYMBOL
#endif

static BridgeEngine_module* module;

#if PY_MAJOR_VERSION == 3

extern "C" EXPORT_SYMBOL PyObject* PyInit_BridgeEngine()
{
#if defined(PY_WIN32_DELAYLOAD_PYTHON_DLL)
	Py::InitialisePythonIndirectPy::Interface();
#endif
	module = new BridgeEngine_module;
	return module->module().ptr();
}

// symbol required for the debug version
extern "C" EXPORT_SYMBOL PyObject* PyInit_BridgeEngine_d()
{
	return PyInit_BridgeEngine();
}

#else

extern "C" EXPORT_SYMBOL void initBridgeEngine()
{
#if defined(PY_WIN32_DELAYLOAD_PYTHON_DLL)
	Py::InitialisePythonIndirectPy::Interface();
#endif

	module = new BridgeEngine_module;
}

// symbol required for the debug version
extern "C" EXPORT_SYMBOL void initBridgeEngine_d()
{
	initBridgeEngine();
}
#endif
