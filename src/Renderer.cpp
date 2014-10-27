#include "Renderer.h"
#define SDL_MAIN_HANDLED // Don't use SDL's main impl
#include "SDL.h"
#include <stdexcept>

struct Renderer::PIMPL
{
	PIMPL()
		: m_window(NULL)
		, m_renderer(NULL)
	{
	}

	SDL_Window* m_window;
	SDL_Renderer* m_renderer;
};

Renderer::Renderer()
	: m_impl(nullptr)
{
}

Renderer::~Renderer()
{
	Destroy();
}

void Renderer::Create()
{
	assert(!m_impl);
	m_impl = new PIMPL();

	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
		throw std::exception("SDL_Init failed");
	
	m_impl->m_window = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1024, 768, SDL_WINDOW_SHOWN );
	if (!m_impl->m_window)
		throw std::exception("SDL_CreateWindow failed");

	m_impl->m_renderer = SDL_CreateRenderer(m_impl->m_window, -1, SDL_RENDERER_ACCELERATED);
	if (!m_impl->m_renderer)
		throw std::exception("SDL_CreateRenderer failed");
}

void Renderer::Destroy()
{
	SDL_DestroyRenderer(m_impl->m_renderer);
	SDL_DestroyWindow(m_impl->m_window);
	delete m_impl;

	m_impl = nullptr;
	m_impl->m_window = nullptr;
	m_impl->m_renderer = nullptr;
}

void Renderer::Clear()
{
	SDL_SetRenderDrawColor(m_impl->m_renderer, 0, 0, 0, 0);
	SDL_RenderClear(m_impl->m_renderer);
}

void Renderer::Render()
{
	SDL_RenderPresent(m_impl->m_renderer);

	// Need to consume all events for window to be responsive
	SDL_Event e;
	while( SDL_PollEvent(&e) )
	{
		if( e.type == SDL_QUIT )
		{
			exit(0);
		}
	}
}

void Renderer::DrawPixel(int x, int y, const Color4& color)
{
	SDL_SetRenderDrawColor(m_impl->m_renderer, color.r, color.g, color.b, color.a);
	SDL_RenderDrawPoint(m_impl->m_renderer, x, y);
}
