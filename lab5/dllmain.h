// dllmain.h : Declaration of module class.

class Clab5Module : public ATL::CAtlDllModuleT< Clab5Module >
{
public :
	DECLARE_LIBID(LIBID_lab5Lib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_LAB5, "{57E5CD6C-A0D6-45F7-AA90-08470428590E}")
};

extern class Clab5Module _AtlModule;
