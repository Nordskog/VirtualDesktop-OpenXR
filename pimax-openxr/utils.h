// MIT License
//
// Copyright(c) 2022 Matthieu Bucchianeri
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this softwareand associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and /or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
//
// The above copyright noticeand this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#pragma once

#include "pch.h"

#define CHECK_PVRCMD(cmd) xr::detail::_CheckPVRResult(cmd, #cmd, FILE_AND_LINE)
#define CHECK_VKCMD(cmd) xr::detail::_CheckVKResult(cmd, #cmd, FILE_AND_LINE)

namespace xr {
    static inline std::string ToString(XrVersion version) {
        return fmt::format("{}.{}.{}", XR_VERSION_MAJOR(version), XR_VERSION_MINOR(version), XR_VERSION_PATCH(version));
    }

    static inline std::string ToString(pvrPosef pose) {
        return fmt::format("p: ({:.3f}, {:.3f}, {:.3f}), o:({:.3f}, {:.3f}, {:.3f}, {:.3f})",
                           pose.Position.x,
                           pose.Position.y,
                           pose.Position.z,
                           pose.Orientation.x,
                           pose.Orientation.y,
                           pose.Orientation.z,
                           pose.Orientation.w);
    }

    static inline std::string ToString(XrPosef pose) {
        return fmt::format("p: ({:.3f}, {:.3f}, {:.3f}), o:({:.3f}, {:.3f}, {:.3f}, {:.3f})",
                           pose.position.x,
                           pose.position.y,
                           pose.position.z,
                           pose.orientation.x,
                           pose.orientation.y,
                           pose.orientation.z,
                           pose.orientation.w);
    }

    static inline std::string ToString(XrFovf fov) {
        return fmt::format(
            "(l:{:.3f}, r:{:.3f}, u:{:.3f}, d:{:.3f})", fov.angleLeft, fov.angleRight, fov.angleUp, fov.angleDown);
    }

    static inline std::string ToString(XrRect2Di rect) {
        return fmt::format("x:{}, y:{} w:{} h:{}", rect.offset.x, rect.offset.y, rect.extent.width, rect.extent.height);
    }

    namespace detail {

        [[noreturn]] static inline void _ThrowPVRResult(pvrResult pvr,
                                                        const char* originator = nullptr,
                                                        const char* sourceLocation = nullptr) {
            xr::detail::_Throw(xr::detail::_Fmt("pvrResult failure [%d]", pvr), originator, sourceLocation);
        }

        static inline HRESULT _CheckPVRResult(pvrResult pvr,
                                              const char* originator = nullptr,
                                              const char* sourceLocation = nullptr) {
            if (pvr != pvr_success) {
                xr::detail::_ThrowPVRResult(pvr, originator, sourceLocation);
            }

            return pvr;
        }

        [[noreturn]] static inline void _ThrowVKResult(VkResult vks,
                                                       const char* originator = nullptr,
                                                       const char* sourceLocation = nullptr) {
            xr::detail::_Throw(xr::detail::_Fmt("VkStatus failure [%d]", vks), originator, sourceLocation);
        }

        static inline HRESULT _CheckVKResult(VkResult vks,
                                             const char* originator = nullptr,
                                             const char* sourceLocation = nullptr) {
            if ((vks) != VK_SUCCESS) {
                xr::detail::_ThrowVKResult(vks, originator, sourceLocation);
            }

            return vks;
        }
    } // namespace detail

} // namespace xr

namespace pimax_openxr::utils {

    // https://docs.microsoft.com/en-us/archive/msdn-magazine/2017/may/c-use-modern-c-to-access-the-windows-registry
    static std::optional<int> RegGetDword(HKEY hKey, const std::string& subKey, const std::string& value) {
        DWORD data{};
        DWORD dataSize = sizeof(data);
        LONG retCode = ::RegGetValue(hKey,
                                     std::wstring(subKey.begin(), subKey.end()).c_str(),
                                     std::wstring(value.begin(), value.end()).c_str(),
                                     RRF_RT_REG_DWORD,
                                     nullptr,
                                     &data,
                                     &dataSize);
        if (retCode != ERROR_SUCCESS) {
            return {};
        }
        return data;
    }

    static std::vector<const char*> ParseExtensionString(char* names) {
        std::vector<const char*> list;
        while (*names != 0) {
            list.push_back(names);
            while (*(++names) != 0) {
                if (*names == ' ') {
                    *names++ = '\0';
                    break;
                }
            }
        }
        return list;
    }

    static inline XrTime pvrTimeToXrTime(double pvrTime) {
        return (XrTime)(pvrTime * 1e9);
    }

    static inline double xrTimeToPvrTime(XrTime xrTime) {
        return xrTime / 1e9;
    }

    static inline XrPosef pvrPoseToXrPose(const pvrPosef& pvrPose) {
        XrPosef xrPose;
        xrPose.position.x = pvrPose.Position.x;
        xrPose.position.y = pvrPose.Position.y;
        xrPose.position.z = pvrPose.Position.z;
        xrPose.orientation.x = pvrPose.Orientation.x;
        xrPose.orientation.y = pvrPose.Orientation.y;
        xrPose.orientation.z = pvrPose.Orientation.z;
        xrPose.orientation.w = pvrPose.Orientation.w;

        return xrPose;
    }

    static inline pvrPosef xrPoseToPvrPose(const XrPosef& xrPose) {
        pvrPosef pvrPose;
        pvrPose.Position.x = xrPose.position.x;
        pvrPose.Position.y = xrPose.position.y;
        pvrPose.Position.z = xrPose.position.z;
        pvrPose.Orientation.x = xrPose.orientation.x;
        pvrPose.Orientation.y = xrPose.orientation.y;
        pvrPose.Orientation.z = xrPose.orientation.z;
        pvrPose.Orientation.w = xrPose.orientation.w;

        return pvrPose;
    }

    static pvrTextureFormat dxgiToPvrTextureFormat(DXGI_FORMAT format) {
        switch (format) {
        case DXGI_FORMAT_R8G8B8A8_UNORM:
            return PVR_FORMAT_R8G8B8A8_UNORM;
        case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
            return PVR_FORMAT_R8G8B8A8_UNORM_SRGB;
        case DXGI_FORMAT_B8G8R8A8_UNORM:
            return PVR_FORMAT_B8G8R8A8_UNORM;
        case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:
            return PVR_FORMAT_B8G8R8A8_UNORM_SRGB;
        case DXGI_FORMAT_B8G8R8X8_UNORM:
            return PVR_FORMAT_B8G8R8X8_UNORM;
        case DXGI_FORMAT_B8G8R8X8_UNORM_SRGB:
            return PVR_FORMAT_B8G8R8X8_UNORM_SRGB;
        case DXGI_FORMAT_R16G16B16A16_FLOAT:
            return PVR_FORMAT_R16G16B16A16_FLOAT;
        case DXGI_FORMAT_D16_UNORM:
            return PVR_FORMAT_D16_UNORM;
        case DXGI_FORMAT_D24_UNORM_S8_UINT:
            return PVR_FORMAT_D24_UNORM_S8_UINT;
        case DXGI_FORMAT_D32_FLOAT:
            return PVR_FORMAT_D32_FLOAT;
        case DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
            return PVR_FORMAT_D32_FLOAT_S8X24_UINT;
        case DXGI_FORMAT_BC1_UNORM:
            return PVR_FORMAT_BC1_UNORM;
        case DXGI_FORMAT_BC1_UNORM_SRGB:
            return PVR_FORMAT_BC1_UNORM_SRGB;
        case DXGI_FORMAT_BC2_UNORM:
            return PVR_FORMAT_BC2_UNORM;
        case DXGI_FORMAT_BC2_UNORM_SRGB:
            return PVR_FORMAT_BC2_UNORM_SRGB;
        case DXGI_FORMAT_BC3_UNORM:
            return PVR_FORMAT_BC3_UNORM;
        case DXGI_FORMAT_BC3_UNORM_SRGB:
            return PVR_FORMAT_BC3_UNORM_SRGB;
        case DXGI_FORMAT_BC6H_UF16:
            return PVR_FORMAT_BC6H_UF16;
        case DXGI_FORMAT_BC6H_SF16:
            return PVR_FORMAT_BC6H_SF16;
        case DXGI_FORMAT_BC7_UNORM:
            return PVR_FORMAT_BC7_UNORM;
        case DXGI_FORMAT_BC7_UNORM_SRGB:
            return PVR_FORMAT_BC7_UNORM_SRGB;
        case DXGI_FORMAT_R11G11B10_FLOAT:
            return PVR_FORMAT_R11G11B10_FLOAT;
        default:
            return PVR_FORMAT_UNKNOWN;
        }
    }

    static pvrTextureFormat vkToPvrTextureFormat(VkFormat format) {
        switch (format) {
        case VK_FORMAT_R8G8B8A8_UNORM:
            return PVR_FORMAT_R8G8B8A8_UNORM;
        case VK_FORMAT_R8G8B8A8_SRGB:
            return PVR_FORMAT_R8G8B8A8_UNORM_SRGB;
        case VK_FORMAT_B8G8R8A8_UNORM:
            return PVR_FORMAT_B8G8R8A8_UNORM;
        case VK_FORMAT_B8G8R8A8_SRGB:
            return PVR_FORMAT_B8G8R8A8_UNORM_SRGB;
        case VK_FORMAT_R16G16B16A16_SFLOAT:
            return PVR_FORMAT_R16G16B16A16_FLOAT;
        case VK_FORMAT_D16_UNORM:
            return PVR_FORMAT_D16_UNORM;
        case VK_FORMAT_D24_UNORM_S8_UINT:
            return PVR_FORMAT_D24_UNORM_S8_UINT;
        case VK_FORMAT_D32_SFLOAT:
            return PVR_FORMAT_D32_FLOAT;
        case VK_FORMAT_D32_SFLOAT_S8_UINT:
            return PVR_FORMAT_D32_FLOAT_S8X24_UINT;
        case VK_FORMAT_BC1_RGBA_UNORM_BLOCK:
            return PVR_FORMAT_BC1_UNORM;
        case VK_FORMAT_BC1_RGBA_SRGB_BLOCK:
            return PVR_FORMAT_BC1_UNORM_SRGB;
        case VK_FORMAT_BC2_UNORM_BLOCK:
            return PVR_FORMAT_BC2_UNORM;
        case VK_FORMAT_BC2_SRGB_BLOCK:
            return PVR_FORMAT_BC2_UNORM_SRGB;
        case VK_FORMAT_BC3_UNORM_BLOCK:
            return PVR_FORMAT_BC3_UNORM;
        case VK_FORMAT_BC3_SRGB_BLOCK:
            return PVR_FORMAT_BC3_UNORM_SRGB;
        case VK_FORMAT_BC6H_UFLOAT_BLOCK:
            return PVR_FORMAT_BC6H_UF16;
        case VK_FORMAT_BC6H_SFLOAT_BLOCK:
            return PVR_FORMAT_BC6H_SF16;
        case VK_FORMAT_BC7_UNORM_BLOCK:
            return PVR_FORMAT_BC7_UNORM;
        case VK_FORMAT_BC7_SRGB_BLOCK:
            return PVR_FORMAT_BC7_UNORM_SRGB;
        case VK_FORMAT_B10G11R11_UFLOAT_PACK32:
            return PVR_FORMAT_R11G11B10_FLOAT;
        default:
            return PVR_FORMAT_UNKNOWN;
        }
    }

    static inline bool isValidSwapchainRect(pvrTextureSwapChainDesc desc, XrRect2Di rect) {
        if (rect.offset.x < 0 || rect.offset.y < 0 || rect.extent.width <= 0 || rect.extent.height <= 0) {
            return false;
        }

        if (rect.offset.x + rect.extent.width > desc.Width || rect.offset.y + rect.extent.height > desc.Height) {
            return false;
        }

        return true;
    }

    static inline void setDebugName(ID3D11DeviceChild* resource, std::string_view name) {
        if (resource && !name.empty()) {
            resource->SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<UINT>(name.size()), name.data());
        }
    }

    static inline void setDebugName(ID3D12Object* resource, std::string_view name) {
        if (resource && !name.empty()) {
            resource->SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<UINT>(name.size()), name.data());
        }
    }
} // namespace pimax_openxr::utils