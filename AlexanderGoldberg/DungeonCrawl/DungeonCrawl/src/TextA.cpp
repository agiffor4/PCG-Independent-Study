#include "TextA.h"
#include "SDL_FontCache.h"
#include <string>
TextA::TextA() {}

TextA::~TextA()
{
	CleanUp();
}
void TextA::InitializeFont(std::string _fontFilePath, int _pointsize, SDL_Renderer* _renderer, SDL_Color _color)
{
	
	SetColor(_color);
	m_fontPath = _fontFilePath;
	m_font = FC_CreateFont();
	m_fontSize = _pointsize;
	m_RenderRef = _renderer;
	FC_LoadFont(m_font, _renderer, _fontFilePath.c_str(), _pointsize, m_color, TTF_STYLE_NORMAL);
	m_fontInitalized = true;
	m_name = m_fontPath;
}



std::string TextA::GetFontPath()
{
	return m_fontPath;
}

void TextA::ResetAfterResize()
{
	
	if (m_font != nullptr)
	{
		FC_FreeFont(m_font);
		m_font = FC_CreateFont();
	}
	
	FC_LoadFont(m_font, m_RenderRef, m_fontPath.c_str(), m_fontSize, m_color, TTF_STYLE_NORMAL);
	
	//FC_ResetFontFromRendererReset(m_font, m_RenderRef, SDL_RENDER_DEVICE_RESET);
}

void TextA::SetText(std::string _text) {
	m_text = _text;
}

SDL_Color TextA::GetColor()
{
	return m_color;
}

void TextA::SetColor(SDL_Color _newColor)
{
	m_color = _newColor;	
}

void TextA::Render(SDL_Renderer* _renderer)
{
	
	if (m_fontInitalized){		
		FC_DrawColor(m_font, _renderer, GetPosition().X, GetPosition().Y, m_color, m_text.c_str());
	}

}
int TextA::getTextWidth(const std::string* _textToUse) {
	
	return FC_GetBounds(m_font, GetPosition().X, GetPosition().Y, FC_AlignEnum::FC_ALIGN_LEFT, FC_MakeScale(1, 1), (_textToUse == nullptr ? m_text.c_str() : _textToUse->c_str())).w;
}
int TextA::getTextHeight(const std::string* _textToUse) {
	return FC_GetBounds(m_font, GetPosition().X, GetPosition().Y, FC_AlignEnum::FC_ALIGN_LEFT, FC_MakeScale(1, 1), (_textToUse == nullptr ? m_text.c_str() : _textToUse->c_str())).h;
}

std::string TextA::GetText()
{
	return m_text;
}

void TextA::CleanUp()
{	
	if (m_font != nullptr)
	{
		FC_FreeFont(m_font);
		m_font = nullptr;
	}
}

unsigned int TextA::GetTextWidth() {
	
	return FC_GetWidth(m_font, m_text.c_str());
}
int TextA::GetFontSize()
{
	
	return m_fontSize;
}
