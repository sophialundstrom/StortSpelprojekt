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

class Renderers : public Singleton<Renderers>
{
private:
	static AnimatedModelRenderer* amr;
	static ColliderRenderer* cr;
	static IDRenderer* idr;
	static ModelRenderer* mr;
	static ParticleRenderer* pr;
	static ShadowRenderer* sr;
	static SkeletonRenderer* skr;
	static TerrainRenderer* tr;
	static VolumeRenderer* vr;
	static WaterRenderer* wr;
public:
	static void InitAnimatedModelRenderer()							{ amr = new AnimatedModelRenderer(); }
	static void InitColliderRenderer()								{ cr = new ColliderRenderer(); }
	static void InitIDRenderer()									{ idr = new IDRenderer(); }
	static void InitModelRenderer(bool isLit = true)				{ mr = new ModelRenderer(isLit); }
	static void InitParticleRenderer()								{ pr = new ParticleRenderer(); }
	static void InitShadowRenderer()								{ sr = new ShadowRenderer(); }
	static void InitSkeletonRenderer()								{ skr = new SkeletonRenderer(); }
	static void InitTerrainRenderer(UINT tesselationAmount = 63)	{ tr = new TerrainRenderer(tesselationAmount); }
	static void InitVolumeRenderer()								{ vr = new VolumeRenderer(); }
	static void InitWaterRenderer(UINT tesselationAmount = 63)		{ wr = new WaterRenderer(tesselationAmount); }

	static AnimatedModelRenderer* AMR() { return amr; }
	static ColliderRenderer* CR()		{ return cr; }
	static IDRenderer* IDR()			{ return idr; }
	static ModelRenderer* MR()			{ return mr; }
	static ParticleRenderer* PR()		{ return pr; }
	static ShadowRenderer* SR()			{ return sr; }
	static SkeletonRenderer* SKR()		{ return skr; }
	static TerrainRenderer* TR()		{ return tr; }
	static VolumeRenderer* VR()			{ return vr; }
	static WaterRenderer* WR()			{ return wr; }

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
	}
	static void Shutdown()
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
	}
};
inline AnimatedModelRenderer*	Renderers::amr	= nullptr;
inline ColliderRenderer*		Renderers::cr	= nullptr;
inline IDRenderer*				Renderers::idr	= nullptr;
inline ModelRenderer*			Renderers::mr	= nullptr;
inline ParticleRenderer*		Renderers::pr	= nullptr;
inline ShadowRenderer*			Renderers::sr	= nullptr;
inline SkeletonRenderer*		Renderers::skr	= nullptr;
inline TerrainRenderer*			Renderers::tr	= nullptr;
inline VolumeRenderer*			Renderers::vr	= nullptr;
inline WaterRenderer*			Renderers::wr	= nullptr;

Renderers& GetRenderersInstance() { return Renderers::Inst(); }
#define RND GetRenderersInstance()

AnimatedModelRenderer* RendererAMR() { return RND.AMR(); }
//ANIMATED MODEL RENDERER
#define AMR RendererAMR()

ColliderRenderer* RendererCR() { return RND.CR(); }
//COLLIDER RENDERER
#define CR RendererCR()

IDRenderer* RendererIDR() { return RND.IDR(); }
//ANIMATED MODEL RENDERER
#define IDR RendererIDR()

ModelRenderer* RendererMR() { return RND.MR(); }
//MODEL RENDERER
#define MR RendererMR()

ParticleRenderer* RendererPR() { return RND.PR(); }
//PARTICLE RENDERER
#define PR RendererPR()

ShadowRenderer* RendererSR() { return RND.SR(); }
//SHADOW RENDERER
#define SR RendererSR()

SkeletonRenderer* RendererSKR() { return RND.SKR(); }
//SKELETON RENDERER
#define SKR RendererSKR()

TerrainRenderer* RendererTR() { return RND.TR(); }
//TERRAIN RENDERER
#define TR RendererTR()

VolumeRenderer* RendererVR() { return RND.VR(); }
//VOLUME RENDERER
#define VR RendererVR()

WaterRenderer* RendererWR() { return RND.WR(); }
//WATER RENDERER
#define WR RendererWR()