#pragma once
#include "AnimatedModelRenderer.h"		//AMR
#include "ColliderRenderer.h"			//CR
#include "IDRenderer.h"					//IDR
#include "ModelRenderer.h"				//MR
#include "ParticleRenderer.h"			//PR
#include "ShadowRenderer.h"				//SR
#include "SkeletonRenderer.h"			//SKR
#include "TerrainRenderer.h"			//TR
#include "VolumeRenderer.h"				//VR
#include "WaterRenderer.h"				//WR
#include "InteractableRenderer.h"		//IR
#include "SkyBoxRenderer.h"				//SBR

class Renderers : public Singleton<Renderers>
{
private:
	static AnimatedModelRenderer* amr;
	static ColliderRenderer* cr;
	static IDRenderer* idr;
	static ModelRenderer* mr;
	static ModelRenderer* smr;
	static ParticleRenderer* pr;
	static ShadowRenderer* sr;
	static SkeletonRenderer* skr;
	static TerrainRenderer* tr;
	static VolumeRenderer* vr;
	static WaterRenderer* wr;
	static InteractableRenderer* ir;
	static SkyBoxRenderer* sbr;
public:
	Renderers() :Singleton(this) {}
	~Renderers() { ShutDown(); }

	static void InitAnimatedModelRenderer()							{ amr = new AnimatedModelRenderer(); }
	static void InitColliderRenderer()								{ cr = new ColliderRenderer(); }
	static void InitIDRenderer()									{ idr = new IDRenderer(); }
	static void InitModelRenderer(bool isLit = true)				{ mr = new ModelRenderer(isLit); }
	static void InitStaticModelRenderer(bool isLit = true)			{ smr = new ModelRenderer(isLit); }
	static void InitParticleRenderer()								{ pr = new ParticleRenderer(); }
	static void InitShadowRenderer()								{ sr = new ShadowRenderer(); }
	static void InitSkeletonRenderer()								{ skr = new SkeletonRenderer(); }
	static void InitTerrainRenderer(float tesselationAmount = 63.0f){ tr = new TerrainRenderer(tesselationAmount); }
	static void InitVolumeRenderer()								{ vr = new VolumeRenderer(); }
	static void InitWaterRenderer(float tesselationAmount = 63.0f)	{ wr = new WaterRenderer(tesselationAmount); }
	static void InitInteractableRenderer()							{ ir = new InteractableRenderer(); }
	static void InitSkyBoxRenderer()								{ sbr = new SkyBoxRenderer(); }

	static AnimatedModelRenderer* AMR() { return amr; }
	static ColliderRenderer* CR()		{ return cr; }
	static IDRenderer* IDR()			{ return idr; }
	static ModelRenderer* MR()			{ return mr; }
	static ModelRenderer* SMR()			{ return smr; }
	static ParticleRenderer* PR()		{ return pr; }
	static ShadowRenderer* SR()			{ return sr; }
	static SkeletonRenderer* SKR()		{ return skr; }
	static TerrainRenderer* TR()		{ return tr; }
	static VolumeRenderer* VR()			{ return vr; }
	static WaterRenderer* WR()			{ return wr; }
	static InteractableRenderer* IR()	{ return ir; }
	static SkyBoxRenderer* SBR()		{ return sbr; }

	static void Clear()
	{
		if (amr)
			amr->Clear();
		if (cr)
			cr->Clear();
		if (idr)
			idr->Clear();
		if (mr)
			mr->Clear();
		if (smr)
			smr->Clear();
		if (pr)
			pr->Clear();
		if (sr)
			sr->Clear();
		if (skr)
			skr->Clear();
		if (vr)
			vr->Clear();
		if (wr)
			wr->Clear();
		if (sbr)
			sbr->Clear();
	}
	static void ShutDown()
	{
		if (amr)
		{
			delete amr;
			amr = nullptr;
		}
			
		if (cr)
		{
			delete cr;
			cr = nullptr;
		}
			
		if (idr)
		{
			delete idr;
			idr = nullptr;
		}
			
		if (mr)
		{
			delete mr;
			mr = nullptr;
		}
			
		if (smr)
		{
			delete smr;
			smr = nullptr;
		}

		if (pr)
		{
			delete pr;
			pr = nullptr;
		}
			
		if (sr)
		{
			delete sr;
			sr = nullptr;
		}
			
		if (skr)
		{
			delete skr;
			skr = nullptr;
		}
			
		if (tr)
		{
			delete tr;
			tr = nullptr;
		}
			
		if (vr)
		{
			delete vr;
			vr = nullptr;
		}
			
		if (wr)
		{
			delete wr;
			wr = nullptr;
		}

		if (sbr)
		{
			delete sbr;
			sbr = nullptr;
		}
	}
};
inline AnimatedModelRenderer*	Renderers::amr	= nullptr;
inline ColliderRenderer*		Renderers::cr	= nullptr;
inline IDRenderer*				Renderers::idr	= nullptr;
inline ModelRenderer*			Renderers::mr	= nullptr;
inline ModelRenderer*			Renderers::smr	= nullptr;
inline ParticleRenderer*		Renderers::pr	= nullptr;
inline ShadowRenderer*			Renderers::sr	= nullptr;
inline SkeletonRenderer*		Renderers::skr	= nullptr;
inline TerrainRenderer*			Renderers::tr	= nullptr;
inline VolumeRenderer*			Renderers::vr	= nullptr;
inline WaterRenderer*			Renderers::wr	= nullptr;
inline InteractableRenderer*	Renderers::ir   = nullptr;
inline SkyBoxRenderer*			Renderers::sbr	= nullptr;

inline Renderers& GetRenderersInstance() { return Renderers::Inst(); }
#define RND GetRenderersInstance()

inline AnimatedModelRenderer* RendererAMR() { return RND.AMR(); }
//ANIMATED MODEL RENDERER
#define AMR RendererAMR()

inline ColliderRenderer* RendererCR() { return RND.CR(); }
//COLLIDER RENDERER
#define CR RendererCR()

inline IDRenderer* RendererIDR() { return RND.IDR(); }
//ANIMATED MODEL RENDERER
#define IDR RendererIDR()

inline ModelRenderer* RendererMR() { return RND.MR(); }
//MODEL RENDERER
#define MR RendererMR()

inline ModelRenderer* RendererSMR() { return RND.SMR(); }
//MODEL RENDERER
#define SMR RendererSMR()

inline ParticleRenderer* RendererPR() { return RND.PR(); }
//PARTICLE RENDERER
#define PR RendererPR()

inline ShadowRenderer* RendererSR() { return RND.SR(); }
//SHADOW RENDERER
#define SR RendererSR()

inline SkeletonRenderer* RendererSKR() { return RND.SKR(); }
//SKELETON RENDERER
#define SKR RendererSKR()

inline TerrainRenderer* RendererTR() { return RND.TR(); }
//TERRAIN RENDERER
#define TR RendererTR()

inline VolumeRenderer* RendererVR() { return RND.VR(); }
//VOLUME RENDERER	
#define VR RendererVR()

inline WaterRenderer* RendererWR() { return RND.WR(); }
//WATER RENDERER
#define WR RendererWR()

inline InteractableRenderer* RendererIR() { return RND.IR(); }
//INTERACTABLE RENDERER
#define IR RendererIR()

inline SkyBoxRenderer* RendererSBR() { return RND.SBR(); }
//SKYBOX RENDERER
#define SBR RendererSBR()