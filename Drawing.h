class BkkDrawing 
{
public:
	void DrawLine(float* fStart, float* fEnd, int lw, int r, int g, int b);
	void DrawLine2(int ax, int ay, int bx, int by, int width, int r, int g, int b, int a);
}; extern BkkDrawing g_Drawing;