#pragma once
#include "ImGui.h"
#include "Graphics.h"
#include "Math.h"

class Settings
{
private:
	static float blendingFactor;
	static float displacementFactor;
	static float tesselationFactor;
	static bool backfaceCulling;
	static bool wireframe;
	static float globalAmbient;
	static float lightAngle;
	static bool updateLight;
	static bool freeCamera;
public:
	static void RenderUI()
	{
		ImGui::Begin("SETTINGS");
		ImGui::SliderFloat("BLENDING", &blendingFactor, 0.0f, 2.0f);
		ImGui::SliderFloat("DISPLACEMENT", &displacementFactor, 0.0f, 1.0f);
		ImGui::SliderFloat("TESSELATION", &tesselationFactor, 1.0f, 63.0f);
		ImGui::Checkbox("BACKFACE CULLING", &backfaceCulling);
		ImGui::Checkbox("WIREFRAME", &wireframe);
		ImGui::SliderFloat("GLOABL AMBIENT", &globalAmbient, 0.0f, 1.0f);
		ImGui::SliderFloat("DIRECTIONAL LIGHT ANGLE", &lightAngle, 0.0f + 0.5, PI - 0.5);
		ImGui::Checkbox("UPDATE DIR LIGHT", &updateLight);
		ImGui::Text("\n --- C TO TOGGLE MOUSE LOCK --- ");
		ImGui::Text(" --- ENTER TO EXIT PROGRAM --- ");
		ImGui::Checkbox("FREE CAMERA", &freeCamera);
		ImGui::End();
	}

	static float BlendingFactor() { return blendingFactor; }
	static float DisplacementFactor() { return displacementFactor; }
	static float TesselationFactor() { return tesselationFactor; }
	static bool UseBackfaceCulling() { return backfaceCulling; }
	static bool UseWireframe() { return wireframe; }
	static float GlobalAmbient() { return globalAmbient; }
	static void SetLightAngle(float value) { lightAngle = value; }
	static float LightAngle() { return lightAngle; }
	static bool UpdateDirectionalLight() { return updateLight; }
	static bool UseFreeCamera() { return freeCamera; }
};

inline float Settings::blendingFactor = 1.3f;
inline float Settings::displacementFactor = 1.0f;
inline float Settings::tesselationFactor = 63.0f;
inline bool Settings::backfaceCulling = true;
inline bool Settings::wireframe = false;
inline float Settings::globalAmbient = 0.4f;
inline float Settings::lightAngle = 0.0f;
inline bool Settings::updateLight = true;
inline bool Settings::freeCamera = true;