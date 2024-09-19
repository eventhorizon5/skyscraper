#include "OgreOpenXRInstance.h"

#include <vector>
#include "XrError.h"
#include "XrDispatchTable.h"

Ogre::OpenXRInstance::OpenXRInstance()
{
}

Ogre::OpenXRInstance::~OpenXRInstance()
{
}


std::vector<const char*> SelectExtensions();

void Ogre::OpenXRInstance::Initialize(const std::string& applicationName)
{
  CHECK(m_instanceHandle.Get() == XR_NULL_HANDLE);

  std::vector<const char*> enabledExtensions = SelectExtensions();

  XrInstanceCreateInfo createInfo{ XR_TYPE_INSTANCE_CREATE_INFO };
  createInfo.enabledExtensionCount = (uint32_t)enabledExtensions.size();
  createInfo.enabledExtensionNames = enabledExtensions.data();
  createInfo.applicationInfo.applicationVersion = 1;
  createInfo.applicationInfo.engineName[0] = 0;
  createInfo.applicationInfo.engineVersion = 1;
  createInfo.applicationInfo.apiVersion = XR_CURRENT_API_VERSION;

  strcpy_s(createInfo.applicationInfo.applicationName, applicationName.c_str());

  CHECK_XRCMD(xrCreateInstance(&createInfo, m_instanceHandle.Put(xrDestroyInstance)));

  xr::g_dispatchTable.Initialize(m_instanceHandle.Get(), xrGetInstanceProcAddr);
}

const xr::InstanceHandle& Ogre::OpenXRInstance::getHandle()
{
  return m_instanceHandle;
}

const bool Ogre::OpenXRInstance::initialized()
{
  return m_instanceHandle.Get() != XR_NULL_HANDLE;
}

std::vector<const char*> SelectExtensions()
{
  // Fetch the list of extensions supported by the runtime.
  uint32_t extensionCount;
  CHECK_XRCMD(xrEnumerateInstanceExtensionProperties(nullptr, 0, &extensionCount, nullptr));
  std::vector<XrExtensionProperties> extensionProperties(extensionCount, { XR_TYPE_EXTENSION_PROPERTIES });
  CHECK_XRCMD(xrEnumerateInstanceExtensionProperties(nullptr, extensionCount, &extensionCount, extensionProperties.data()));

  std::vector<const char*> enabledExtensions;

  // Add a specific extension to the list of extensions to be enabled, if it is supported.
  auto EnableExtensionIfSupported = [&](const char* extensionName) {
    for (uint32_t i = 0; i < extensionCount; i++) {
      if (strcmp(extensionProperties[i].extensionName, extensionName) == 0) {
        enabledExtensions.push_back(extensionName);
        return true;
      }
    }
    return false;
  };

  // D3D11 extension is required for this sample, so check if it's supported.
  CHECK(EnableExtensionIfSupported(XR_KHR_D3D11_ENABLE_EXTENSION_NAME));

  return enabledExtensions;
}