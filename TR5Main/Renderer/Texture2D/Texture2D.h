#pragma once
#include <d3d11.h>
#include <string>
#include <wrl/client.h>
namespace T5M::Renderer {
	using Microsoft::WRL::ComPtr;
	class Texture2D {
	public:
		ComPtr<ID3D11ShaderResourceView> ShaderResourceView;
		ComPtr<ID3D11Texture2D> Texture;
		Texture2D() {};
		Texture2D(ID3D11Device* device, int w, int h, byte* data);
		Texture2D(ID3D11Device* device, const std::wstring& fileName);

		~Texture2D() {}

		Texture2D(ID3D11Device* device, byte* data, int length);
	};

}