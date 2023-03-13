#include "pch.h"
#include "LibGraph.h"



using namespace Gdiplus;

char* contents = NULL;
long fsize;
int border = 2;

extern int fisheye_view;
extern char* G_title;
extern int G_color;
extern int G_displayel;
extern int G_stretch, G_shrink;
extern long V_xmin, V_ymin;
extern long fe_scaling;
extern long gfocus_x, gfocus_y;

extern CMFCStatusBar *m_pStatusBar;


void error(const char* format, ...) {
    char errbuf[1024];
    va_list va;
    va_start(va, format);
    vsnprintf(errbuf, sizeof(errbuf), format, va);
    va_end(va);
    MessageBoxA(NULL, "Error", errbuf,MB_OK);
    exit(-1);
}


void save_input_file_contents(FILE* fp) {
    fseek(fp, 0, SEEK_END);
    fsize = ftell(fp);
    if (fsize > 0) {
        contents = (char*)malloc(fsize);
        if (contents == NULL) {
            error("Not enough memory for the input file");
        }
        fseek(fp, 0, SEEK_SET);
        fsize = fread(contents, 1, fsize, fp);
    }
    fseek(fp, 0, SEEK_SET);
}


//   void cleanup_streams(void);
void __cdecl setFeScaling(int stretch, int shrink)
{
    set_fe_scaling(stretch, shrink);
}
void __cdecl doStatistics(void)
{
    statistics();
}


void __cdecl displayCompleteGraph(void) {
    display_complete_graph();
}

int __cdecl normalFeFocus(void)
{
    return normal_fe_focus();
}

int __cdecl vcgMain(int argc, char* argv[])
{
   return  vcg_main(argc, argv);
}

void __cdecl fishEyeMode(int code)
{
    m_validate_fe(code);
}
void __cdecl relayOut(void)
{
    relayout();
}

void __cdecl setOriginValue(const char* _origin)
{
    //wchar_t txtbuf[4096];

    assert(m_pStatusBar);

   // mbtowc(txtbuf, _origin, strlen(_origin));
    m_pStatusBar->SetPaneText(1, _origin);

}
int __cdecl setDrawingRectangle(int width, int height)
{
   return set_drawing_rectangle(width, height);
}

void __cdecl updateOriginStatus(void)
{
    char buf[80];
    if (fisheye_view != 0) {
        _snprintf(buf, sizeof(buf), "(%ld,%ld)", gfocus_x, gfocus_y);
    }
    else {
        _snprintf(buf, sizeof(buf), "(%ld,%ld)", -V_xmin, -V_ymin);
    }
    setOriginValue( buf);

}
void __cdecl setZoomValue(const char* _zoom)
{
    //wchar_t txtbuf[256];

    assert(m_pStatusBar);

    //mbtowc(txtbuf, _zoom, strlen(_zoom));
    m_pStatusBar->SetPaneText(0, _zoom);

}

void __cdecl updateZoomStatus(void)
{
    char buf[80];
    double zoom;
    if (fisheye_view != 0) {
        zoom = fe_scaling;
    }
    else {
        zoom = double(G_stretch) * 100;
        if (G_shrink != 0) zoom /= G_shrink;
    }
    _snprintf(buf, sizeof(buf), "%5.2f%%", zoom);
    setZoomValue(buf);
    updateOriginStatus();
}




void __cdecl moveFocus(int dx, int dy)
{
    incr_fe_focus(dx, dy);
    updateOriginStatus();
}

Gdiplus::Color vcg2bcc(int color) 
{
    if (color < 256) {
        int r = redmap[color];
        int g = greenmap[color];
        int b = bluemap[color];
        return Gdiplus::Color(r, g, b);
    }
    return Gdiplus::Color(255, 0, 0);
}

void draw_line(int x1, int y1, int x2, int y2, int color, void* painter) 
{
    Graphics* g = (Graphics*)painter;
    x1 += border;
    x2 += border;
    y1 += border;
    y2 += border;

    Gdiplus::Pen pen(vcg2bcc(color));
    g->DrawLine(&pen, x1, y1, x2, y2);

}

void draw_rect(long x, long y, int w, int h, int color, void* painter) 
{
    Graphics* g = (Graphics*)painter;

    x += border;
    y += border;

    
    Gdiplus::SolidBrush brush(vcg2bcc(color));
    g->FillRectangle(&brush, x, y, w, h);
    
    Gdiplus::Pen pen(vcg2bcc(color), 1);
    g->DrawRectangle(&pen, x, y, w, h);
}


void draw_poly(POINT* hp, int n, int color, void* painter) 
{
    Graphics* g = (Graphics*)painter;

    // Create a Pen object.
    //Gdiplus::Pen pen(vcg2bcc(color), 3);

    PointF* pts = new PointF[n];
    for (int i = 0; i < n; i++) {
        pts[i].X = (REAL)(hp[i].x + border);
        pts[i].Y = (REAL)(hp[i].y + border);
    }
    Gdiplus::Pen pen(vcg2bcc(color), 1);
    Gdiplus::SolidBrush brush(vcg2bcc(color));

    g->DrawPolygon(&pen, pts,n);

    delete[] pts;
}


char* qtmpnam(char* answer) 
{
    // returns temporary file name
    // (abs path, includes directory, uses TEMP/TMP vars) {
    static int n = -1;

    const char* dir = getenv("TEMP");
    if (dir == NULL) {
        dir = getenv("TMP");
        if (dir == NULL) dir = "C:\\TEMP";
    }

    if (n == -1) n = time(NULL) * 1000;

    char* ptr = strcpy(answer, dir);
    ptr += strlen(ptr);
    if (ptr[-1] != '\\' && ptr[-1] != '/') {
        *ptr++ = '\\';
    }
    strcpy(ptr, "vcg12345.tmp");

    char* change = strchr(answer, 0) - 9;
    while (1) {
        char buf2[20];
        _snprintf(buf2, sizeof(buf2), "%05u", n);
        int len = strlen(buf2);
        char* ptr = buf2 + len - 5;
        change[0] = ptr[0];
        change[1] = ptr[1];
        change[2] = ptr[2];
        change[3] = ptr[3];
        change[4] = ptr[4];
        if (_access(answer, 0)) break;
        n++;
    }
    return answer;
}

//---------------------------------------------------------------------------
/*
static char stderr_file[MAX_PATH]; long stderr_pos;
static char stdout_file[MAX_PATH]; long stdout_pos;

long display_stream(FILE *fp, long pos) {
  long np = ftell(fp);
  int len = np - pos;
  if ( len > 0 )
  {
    fseek(fp, pos, SEEK_SET);
    char *buf = new char[len+1];
    fread(buf, len, 1, fp);
    buf[len] = '\0';
    Form1->RichEdit1->Text = Form1->RichEdit1->Text + buf;
    Form1->RichEdit1->SelStart = Form1->RichEdit1->Text.Length() - 2;
    SendMessage(Form1->RichEdit1->Handle, EM_SCROLLCARET, 0, 0);
    delete buf;
    if ( Form1->Panel1->Height == 0 ) Form1->Panel1->Height = 100;
  }
  return np;
}

void display_streams(void) {
  stdout_pos = display_stream(stdout, stdout_pos);
  stderr_pos = display_stream(stderr, stderr_pos);
  Form1->Update();
}

void cleanup_streams(void) {
  fclose(stderr); unlink(stderr_file);
  fclose(stdout); unlink(stdout_file);
}
*/

void __cdecl setNodesValue(const char* _nodes)
{
    //wchar_t txtbuf[4096];


    assert(m_pStatusBar);

    //mbtowc(txtbuf, _nodes, strlen(_nodes));
    m_pStatusBar->SetPaneText(2, _nodes);

}


void __cdecl drawGraph(void* g)
{
    draw_graph(draw_line, draw_rect, draw_poly, g);
}

void __cdecl changeFeScaling(int stretch, int shrink)
{
    change_fe_scaling(stretch, shrink);
}