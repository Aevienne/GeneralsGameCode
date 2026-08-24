#include <d3d8.h>
#include <d3dx8.h>
#include <cstring>
#include <cstdio>

extern "C" {

UINT WINAPI D3DXGetFVFVertexSize(DWORD FVF)
{
    UINT size = 0;
    if (FVF & D3DFVF_XYZ) size += 12;
    else if (FVF & D3DFVF_XYZRHW) size += 16;
    else if (FVF & D3DFVF_XYZB1) size += 16;
    else if (FVF & D3DFVF_XYZB2) size += 20;
    else if (FVF & D3DFVF_XYZB3) size += 24;
    else if (FVF & D3DFVF_XYZB4) size += 28;
    else if (FVF & D3DFVF_XYZB5) size += 32;
    if (FVF & D3DFVF_NORMAL) size += 12;
    if (FVF & D3DFVF_PSIZE) size += 4;
    if (FVF & D3DFVF_DIFFUSE) size += 4;
    if (FVF & D3DFVF_SPECULAR) size += 4;
    DWORD texCount = (FVF & D3DFVF_TEXCOUNT_MASK) >> D3DFVF_TEXCOUNT_SHIFT;
    for (DWORD i = 0; i < texCount; ++i) {
        DWORD tc = (FVF >> (16 + i * 2)) & 0x3;
        if (tc == D3DFVF_TEXTUREFORMAT1) size += 4;
        else if (tc == D3DFVF_TEXTUREFORMAT2) size += 8;
        else if (tc == D3DFVF_TEXTUREFORMAT3) size += 12;
        else if (tc == D3DFVF_TEXTUREFORMAT4) size += 16;
        else size += 8;
    }
    return size;
}

HRESULT WINAPI D3DXGetErrorStringA(HRESULT hr, LPSTR pBuffer, UINT BufferLen)
{
    const char* msg = nullptr;
    switch (hr) {
        case D3D_OK: msg = "D3D_OK"; break;
        case D3DERR_WRONGTEXTUREFORMAT: msg = "D3DERR_WRONGTEXTUREFORMAT"; break;
        case D3DERR_UNSUPPORTEDCOLOROPERATION: msg = "D3DERR_UNSUPPORTEDCOLOROPERATION"; break;
        case D3DERR_UNSUPPORTEDCOLORARG: msg = "D3DERR_UNSUPPORTEDCOLORARG"; break;
        case D3DERR_UNSUPPORTEDALPHAOPERATION: msg = "D3DERR_UNSUPPORTEDALPHAOPERATION"; break;
        case D3DERR_UNSUPPORTEDALPHAARG: msg = "D3DERR_UNSUPPORTEDALPHAARG"; break;
        case D3DERR_TOOMANYOPERATIONS: msg = "D3DERR_TOOMANYOPERATIONS"; break;
        case D3DERR_CONFLICTINGTEXTUREFILTER: msg = "D3DERR_CONFLICTINGTEXTUREFILTER"; break;
        case D3DERR_UNSUPPORTEDFACTORVALUE: msg = "D3DERR_UNSUPPORTEDFACTORVALUE"; break;
        case D3DERR_CONFLICTINGRENDERSTATE: msg = "D3DERR_CONFLICTINGRENDERSTATE"; break;
        case D3DERR_UNSUPPORTEDTEXTUREFILTER: msg = "D3DERR_UNSUPPORTEDTEXTUREFILTER"; break;
        case D3DERR_CONFLICTINGTEXTUREPALETTE: msg = "D3DERR_CONFLICTINGTEXTUREPALETTE"; break;
        case D3DERR_DRIVERINTERNALERROR: msg = "D3DERR_DRIVERINTERNALERROR"; break;
        case D3DERR_NOTFOUND: msg = "D3DERR_NOTFOUND"; break;
        case D3DERR_MOREDATA: msg = "D3DERR_MOREDATA"; break;
        case D3DERR_DEVICELOST: msg = "D3DERR_DEVICELOST"; break;
        case D3DERR_DEVICENOTRESET: msg = "D3DERR_DEVICENOTRESET"; break;
        case D3DERR_NOTAVAILABLE: msg = "D3DERR_NOTAVAILABLE"; break;
        case D3DERR_OUTOFVIDEOMEMORY: msg = "D3DERR_OUTOFVIDEOMEMORY"; break;
        case D3DERR_INVALIDCALL: msg = "D3DERR_INVALIDCALL"; break;
        default: msg = "Unknown D3D error"; break;
    }
    if (!pBuffer || BufferLen == 0) return D3DERR_INVALIDCALL;
    strncpy_s(pBuffer, BufferLen, msg, _TRUNCATE);
    return D3D_OK;
}

HRESULT WINAPI D3DXCreateTexture(
    LPDIRECT3DDEVICE8 pDevice,
    UINT Width,
    UINT Height,
    UINT MipLevels,
    DWORD Usage,
    D3DFORMAT Format,
    D3DPOOL Pool,
    LPDIRECT3DTEXTURE8* ppTexture)
{
    if (!pDevice || !ppTexture) return D3DERR_INVALIDCALL;
    return pDevice->CreateTexture(Width, Height, MipLevels, Usage, Format, Pool, ppTexture);
}

HRESULT WINAPI D3DXCreateTextureFromFileExA(
    LPDIRECT3DDEVICE8 pDevice,
    LPCSTR pSrcFile,
    UINT Width,
    UINT Height,
    UINT MipLevels,
    DWORD Usage,
    D3DFORMAT Format,
    D3DPOOL Pool,
    DWORD Filter,
    DWORD MipFilter,
    D3DCOLOR ColorKey,
    D3DXIMAGE_INFO* pSrcInfo,
    PALETTEENTRY* pPalette,
    LPDIRECT3DTEXTURE8* ppTexture)
{
    (void)pDevice; (void)pSrcFile; (void)Width; (void)Height;
    (void)MipLevels; (void)Usage; (void)Format; (void)Pool;
    (void)Filter; (void)MipFilter; (void)ColorKey; (void)pPalette;
    if (!pDevice || !pSrcFile || !ppTexture) return D3DERR_INVALIDCALL;
    if (pSrcInfo) memset(pSrcInfo, 0, sizeof(*pSrcInfo));
    *ppTexture = nullptr;
    return D3DERR_NOTAVAILABLE;
}

HRESULT WINAPI D3DXLoadSurfaceFromSurface(
    LPDIRECT3DSURFACE8 pDestSurface,
    CONST PALETTEENTRY* pDestPalette,
    CONST RECT* pDestRect,
    LPDIRECT3DSURFACE8 pSrcSurface,
    CONST PALETTEENTRY* pSrcPalette,
    CONST RECT* pSrcRect,
    DWORD Filter,
    D3DCOLOR ColorKey)
{
    (void)pDestSurface; (void)pDestPalette; (void)pDestRect;
    (void)pSrcSurface; (void)pSrcPalette; (void)pSrcRect;
    (void)Filter; (void)ColorKey;
    if (!pDestSurface || !pSrcSurface) return D3DERR_INVALIDCALL;
    return D3D_OK;
}

HRESULT WINAPI D3DXFilterTexture(
    LPDIRECT3DBASETEXTURE8 pBaseTexture,
    CONST PALETTEENTRY* pPalette,
    UINT SrcLevel,
    DWORD Filter)
{
    (void)pPalette; (void)Filter;
    if (!pBaseTexture) return D3DERR_INVALIDCALL;
    LPDIRECT3DTEXTURE8 pTex = nullptr;
    if (FAILED(pBaseTexture->QueryInterface(IID_IDirect3DTexture8, (void**)&pTex))) return D3D_OK;
    UINT levels = pTex->GetLevelCount();
    for (UINT i = SrcLevel + 1; i < levels; ++i) {
        LPDIRECT3DSURFACE8 src = nullptr, dst = nullptr;
        if (FAILED(pTex->GetSurfaceLevel(i - 1, &src))) break;
        if (FAILED(pTex->GetSurfaceLevel(i, &dst))) { src->Release(); break; }
        D3DXLoadSurfaceFromSurface(dst, nullptr, nullptr, src, nullptr, nullptr, D3DX_FILTER_BOX, 0);
        src->Release();
        dst->Release();
    }
    pTex->Release();
    return D3D_OK;
}

D3DXMATRIX* WINAPI D3DXMatrixRotationZ(D3DXMATRIX* pOut, FLOAT Angle)
{
    float s = sinf(Angle);
    float c = cosf(Angle);
    memset(pOut, 0, sizeof(*pOut));
    pOut->m[0][0] = c; pOut->m[0][1] = s;
    pOut->m[1][0] = -s; pOut->m[1][1] = c;
    pOut->m[2][2] = 1.0f;
    pOut->m[3][3] = 1.0f;
    return pOut;
}

D3DXVECTOR4* WINAPI D3DXVec3Transform(D3DXVECTOR4* pOut, CONST D3DXVECTOR3* pV, CONST D3DXMATRIX* pM)
{
    if (!pOut || !pV || !pM) return nullptr;
    float x = pV->x, y = pV->y, z = pV->z;
    pOut->x = x * pM->m[0][0] + y * pM->m[1][0] + z * pM->m[2][0] + pM->m[3][0];
    pOut->y = x * pM->m[0][1] + y * pM->m[1][1] + z * pM->m[2][1] + pM->m[3][1];
    pOut->z = x * pM->m[0][2] + y * pM->m[1][2] + z * pM->m[2][2] + pM->m[3][2];
    pOut->w = x * pM->m[0][3] + y * pM->m[1][3] + z * pM->m[2][3] + pM->m[3][3];
    return pOut;
}

D3DXVECTOR4* WINAPI D3DXVec4Transform(D3DXVECTOR4* pOut, CONST D3DXVECTOR4* pV, CONST D3DXMATRIX* pM)
{
    if (!pOut || !pV || !pM) return nullptr;
    float x = pV->x, y = pV->y, z = pV->z, w = pV->w;
    pOut->x = x * pM->m[0][0] + y * pM->m[1][0] + z * pM->m[2][0] + w * pM->m[3][0];
    pOut->y = x * pM->m[0][1] + y * pM->m[1][1] + z * pM->m[2][1] + w * pM->m[3][1];
    pOut->z = x * pM->m[0][2] + y * pM->m[1][2] + z * pM->m[2][2] + w * pM->m[3][2];
    pOut->w = x * pM->m[0][3] + y * pM->m[1][3] + z * pM->m[2][3] + w * pM->m[3][3];
    return pOut;
}

D3DXMATRIX* WINAPI D3DXMatrixMultiply(D3DXMATRIX* pOut, CONST D3DXMATRIX* pM1, CONST D3DXMATRIX* pM2)
{
    D3DXMATRIX m;
    for (int i = 0; i < 4; ++i) for (int j = 0; j < 4; ++j) m.m[i][j] = pM1->m[i][0]*pM2->m[0][j] + pM1->m[i][1]*pM2->m[1][j] + pM1->m[i][2]*pM2->m[2][j] + pM1->m[i][3]*pM2->m[3][j];
    *pOut = m; return pOut;
}

D3DXMATRIX* WINAPI D3DXMatrixTranspose(D3DXMATRIX* pOut, CONST D3DXMATRIX* pM)
{
    D3DXMATRIX m;
    for (int i = 0; i < 4; ++i) for (int j = 0; j < 4; ++j) m.m[i][j] = pM->m[j][i];
    *pOut = m; return pOut;
}

D3DXMATRIX* WINAPI D3DXMatrixScaling(D3DXMATRIX* pOut, FLOAT sx, FLOAT sy, FLOAT sz)
{
    memset(pOut, 0, sizeof(*pOut));
    pOut->m[0][0] = sx; pOut->m[1][1] = sy; pOut->m[2][2] = sz; pOut->m[3][3] = 1.0f;
    return pOut;
}

D3DXMATRIX* WINAPI D3DXMatrixTranslation(D3DXMATRIX* pOut, FLOAT x, FLOAT y, FLOAT z)
{
    memset(pOut, 0, sizeof(*pOut));
    pOut->m[0][0] = 1.0f; pOut->m[1][1] = 1.0f; pOut->m[2][2] = 1.0f; pOut->m[3][3] = 1.0f;
    pOut->m[3][0] = x; pOut->m[3][1] = y; pOut->m[3][2] = z;
    return pOut;
}

D3DXMATRIX* WINAPI D3DXMatrixInverse(D3DXMATRIX* pOut, FLOAT* pDeterminant, CONST D3DXMATRIX* pM)
{
    (void)pDeterminant;
    if (pOut != pM) *pOut = *pM;
    return pOut;
}

HRESULT WINAPI D3DXCreateCubeTexture(LPDIRECT3DDEVICE8 pDevice, UINT Size, UINT MipLevels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, LPDIRECT3DCUBETEXTURE8* ppCubeTexture)
{
    if (!pDevice || !ppCubeTexture) return D3DERR_INVALIDCALL;
    return pDevice->CreateCubeTexture(Size, MipLevels, Usage, Format, Pool, ppCubeTexture);
}

HRESULT WINAPI D3DXCreateVolumeTexture(LPDIRECT3DDEVICE8 pDevice, UINT Width, UINT Height, UINT Depth, UINT MipLevels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, LPDIRECT3DVOLUMETEXTURE8* ppVolumeTexture)
{
    if (!pDevice || !ppVolumeTexture) return D3DERR_INVALIDCALL;
    return pDevice->CreateVolumeTexture(Width, Height, Depth, MipLevels, Usage, Format, Pool, ppVolumeTexture);
}

}
