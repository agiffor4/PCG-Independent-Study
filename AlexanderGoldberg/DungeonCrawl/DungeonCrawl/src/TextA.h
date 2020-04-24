#pragma once
#include "Renderable.h"
struct FC_Font;
class TextA :
	public Renderable
{
protected:
	SDL_Renderer* m_RenderRef = nullptr;
	bool m_fontInitalized = false;
	FC_Font* m_font = nullptr;
	std::string m_fontPath;
	int m_fontSize = 12;
	std::string m_text = "";
	SDL_Color m_color;
public:
	TextA();
	~TextA();
	void InitializeFont(std::string _fontFilePath, int _pointsize, SDL_Renderer* _renderer, SDL_Color _color);
	unsigned int GetTextWidth();
	int GetFontSize();
	std::string GetFontPath();
	virtual void ResetAfterResize();
	void SetText(std::string _text);
	SDL_Color GetColor();
	void SetColor(SDL_Color _newColor);
	void CleanUp() override;
	void Render(SDL_Renderer* _renderer) override;
	int getTextWidth(const std::string* _textToUse = nullptr);
	int getTextHeight(const std::string* _textToUse = nullptr);
	std::string GetText();
};

