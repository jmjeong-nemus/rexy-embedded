#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include "rexy.h"
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>

#include "application.h" // for fontId

#define DEVICE_WIDTH 240
#define DEVICE_HEIGHT 320
extern UNITCL lcd_bmp_buffer[DEVICE_HEIGHT][DEVICE_WIDTH];
extern GAsyncQueue* RalKernelBuffer;

static GtkWidget *main_canvas = NULL;
static guchar frame_buffer[DEVICE_HEIGHT][DEVICE_WIDTH][4];
static int timer_id = -1;
static int rexy_pid;

bool RxGdkSetTimer(ruint32 timeval, int id);
bool RxGdkKillTimer(int id);

void make_frame(GtkWidget* drawing);

void SetFrameBuffer(rect* inBoundary)
{
     int x, y;
     int sx, sy;
     int ex, ey;		

     // revised as pixel position concept by Joshua
     // initialization
     sx = 0;
     sy = 0;
     ex = DEVICE_WIDTH -1;	
     ey = DEVICE_HEIGHT-1;

     if ( inBoundary )
     {

	  //printf(" partial: top:%d,left:%d,right:%d,bottom:%d",inBoundary->top,inBoundary->left,inBoundary->right,inBoundary->bottom);

	  if ( inBoundary->left > sx ) sx = inBoundary->left;
	  if ( inBoundary->right < ex ) ex = inBoundary->right;
	  if ( inBoundary->top > sy ) sy = inBoundary->top;
	  if ( inBoundary->bottom < ey ) ey = inBoundary->bottom;
 
	
     }
     //printf("\n");

     gdk_threads_enter();

     for( y = sy ; y <= ey ; y++ )
     {
	  for ( x = sx ; x <= ex ; x++ )
	  {
	       int r, g, b, data;
	       data = lcd_bmp_buffer[y][x];
	       r = (data&0xF800)>>8;
	       g = (data&0x07E0)>>3;
	       b = (data&0x001F)<<3;
	       frame_buffer[y][x][0] = r;
	       frame_buffer[y][x][1] = g;
	       frame_buffer[y][x][2] = b;
	  }
     }
     gdk_draw_rgb_32_image(main_canvas->window,
			   main_canvas->style->fg_gc[GTK_WIDGET_STATE(main_canvas)],
			   0, 0,
			   240, 320,
			   GDK_RGB_DITHER_NONE,
			   frame_buffer,
			   240*4);
     gdk_threads_leave();
}

gboolean timeout_function(gpointer data)
{
     pmsg  sndMsg;
     
     gdk_threads_enter();

     printf("Got Kernel Timer id :%d\n",data);

     sndMsg = malloc( sizeof(msg) );
     sndMsg->wmsg = AWM_TIMER;
     sndMsg->wparam = data;
     sndMsg->lparam = 0;

     g_async_queue_push( RalKernelBuffer, sndMsg );
     gdk_threads_leave();

     timer_id = -1;
     return FALSE;	// do not repeat this timer
}

bool RxGdkSetTimer(ruint32 timeval, int id)
{
     int ret;

     gdk_threads_enter();

     if ( timer_id != -1 ) g_source_remove(timer_id);
     timer_id = -1;

     ret = g_timeout_add (timeval,timeout_function,(gpointer)id);
     timer_id = ret;

     //printf("RxGdkSetTimer: g_timeout_add with id :%d  interval :%ld  returned %d\n", id, timeval, ret);

     gdk_threads_leave();
     return FALSE;
}

bool RxGdkKillTimer(int id)
{
     gint ret;

     //printf("RxGdkKillTimer id :%d\n",id);

     gdk_threads_enter();

     if ( timer_id != -1 )
     {
	  ret = g_source_remove (timer_id);
	  timer_id = -1;
	  //printf("KillTimer %d\n", ret);
     }

     gdk_threads_leave();

     return TRUE;
}


void destroy(GtkWidget* widget, gpointer data)
{
     pmsg sndMsg;

     //printf("Got Kernel Timer id :%d\n",data);
     sndMsg = malloc(sizeof(msg));
     sndMsg->wmsg = AWM_QUIT;
     sndMsg->wparam = 1;
     sndMsg->lparam = 0;

     g_async_queue_push( RalKernelBuffer, sndMsg );

     gtk_main_quit();
}

gboolean key_press_event(GtkWidget *widget, GdkEventKey *event, gpointer user_data)
{
     int ret;
     guint key;
     pmsg	sndMsg;

     //printf("key_press_event %X\n", event->keyval);
     switch( event->keyval )
     {
     case GDK_Up: 	key = VK_USER_UP; break;
     case GDK_Down: 	key = VK_USER_DOWN; break;
     case GDK_Right: 	key = VK_USER_RIGHT; break;
     case GDK_Left: 	key = VK_USER_LEFT; break;
     case GDK_Return: 	key = VK_USER_OK; break;
     case GDK_Escape: 	key = VK_USER_CLR; break;
     case GDK_0: 	key = VK_PAD_0; break;
     case GDK_1:	key = VK_PAD_1; break;
     case GDK_2:	key = VK_PAD_2; break;
     case GDK_3:	key = VK_PAD_3; break;
     case GDK_4:	key = VK_PAD_4; break;
     case GDK_5:	key = VK_PAD_5; break;
     case GDK_6:	key = VK_PAD_6; break;
     case GDK_7:	key = VK_PAD_7; break;
     case GDK_8:	key = VK_PAD_8; break;
     case GDK_9:	key = VK_PAD_9; break;     
     
     default:
	  return FALSE;
     }

     //g_print("key clicked: %d\n", key);

     if ( !RalKernelBuffer ) return;

     sndMsg = malloc( sizeof(msg));
     sndMsg->wmsg = AWM_KEYDOWN;
     sndMsg->wparam = key;
     sndMsg->lparam = 0;

     g_async_queue_push( RalKernelBuffer, sndMsg );
     return TRUE;
}

void button_clicked(GtkWidget* widget, gpointer data)
{
     int ret;
     guint num;
     pmsg	sndMsg;

     num  = (guint)data;
     g_print("clicked: %d\n", num);


     if ( !RalKernelBuffer ) return;

     sndMsg = malloc(sizeof(msg));
     sndMsg->wmsg = AWM_KEYDOWN;
     sndMsg->wparam = '0'+num;
     sndMsg->lparam = 0;

     g_async_queue_push( RalKernelBuffer, sndMsg );
}

void make_frame(GtkWidget* drawing)
{
     int x, y;
     for( y = 0 ; y < 320 ; y++ )
     {
	  for ( x = 0 ; x < 240 ; x++ )
	  {
	       int r, g, b, data;
	       data = lcd_bmp_buffer[y][x];
	       r = (data&0xF800)>>8;
	       g = (data&0x07E0)>>3;
	       b = (data&0x001F)<<3;
	       frame_buffer[y][x][0] = r;
	       frame_buffer[y][x][1] = g;
	       frame_buffer[y][x][2] = b;
	  }
     }

     return;
}

static gboolean draw_frame(GtkWidget* widget, GdkEventExpose* event)
{
     gdk_draw_rgb_32_image(main_canvas->window,
			   main_canvas->style->fg_gc[GTK_WIDGET_STATE(main_canvas)],
			   0, 0,
			   240, 320,
			   GDK_RGB_DITHER_NONE,
			   frame_buffer,
			   240*4);
     return FALSE;
}

gboolean build_device(void)
{
     GtkWidget *window;
     GtkWidget *button;
     GtkWidget *box;
     GtkWidget *table;
     int i,j;

     window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
     gtk_signal_connect(GTK_OBJECT(window), "destroy",
			GTK_SIGNAL_FUNC(destroy), NULL);
     gtk_signal_connect(GTK_OBJECT(window), "key-press-event",
			GTK_SIGNAL_FUNC(key_press_event), NULL);
     box = gtk_vbox_new(FALSE,0);
     gtk_container_add(GTK_CONTAINER(window), box);

     main_canvas = gtk_drawing_area_new();
     gtk_drawing_area_size( (GtkDrawingArea*)main_canvas, 240, 320 );
     gtk_signal_connect(GTK_OBJECT(main_canvas), "configure_event",
			(GtkSignalFunc)make_frame, NULL);
     gtk_signal_connect(GTK_OBJECT(main_canvas), "expose_event",
			(GtkSignalFunc)draw_frame, NULL);
     gtk_widget_set_events( main_canvas, GDK_EXPOSURE_MASK);
     gtk_widget_show(main_canvas);
     gtk_box_pack_start(GTK_BOX(box), main_canvas, TRUE, TRUE, 0);

     table = gtk_table_new( 3, 3, TRUE);
     gtk_box_pack_start(GTK_BOX(box), table, TRUE, TRUE, 0);
     for ( j = 0 ; j < 3 ; j++ )
     {
	  for ( i = 0 ; i < 3 ; i++ )
	  {
	       char tmp[4];
	       sprintf( tmp, "%d", j*3+i );
	       button = gtk_button_new_with_label(tmp);
	       gtk_signal_connect(GTK_OBJECT(button), "clicked",
				  GTK_SIGNAL_FUNC(button_clicked), (gpointer)(j*3+i));
	       gtk_signal_connect(GTK_OBJECT(button), "key-press-event",
				  GTK_SIGNAL_FUNC(key_press_event), NULL);
	       gtk_table_attach_defaults(GTK_TABLE(table), button, i, i+1, j, j+1);
	       gtk_widget_show(button);
	  }
     }
     gtk_widget_show(table);

     gtk_widget_show(box);
     gtk_widget_show(window);

}

void dump_image(int width, int height, int byteWidth, void* buffer)
{
     int x,y,z;
     for ( y = 0 ; y < height ; y++ )
     {
	  ruint16 data;
	  if ( byteWidth == 1 )
	  {
	       data = *(ruint8*)(buffer + byteWidth * y);
	  }
	  else if ( byteWidth == 2 )
	  {
	       data = *(ruint16*)(buffer + byteWidth * y);
	  }

	  for ( x = 0 ; x < width ;x++ )
	  {
	       lcd_bmp_buffer[y][x] = (data & ( 1<<x )) ? 0xFFFF:0x0000;
	  }
     }

}

static const int gridWidth = 10;

void draw_box(int ox, int oy)
{
     int x, y;
     for ( x = gridWidth * ox ; x < gridWidth * (ox+1) ; x++ )
     {
	  for ( y = gridWidth * oy ; y < gridWidth * (oy+1); y++ )
	  {
	       lcd_bmp_buffer[y][x] = 0xFFFF;
	  }
     }
}

void draw_grid(void)
{
     int x, y;
     for ( x = 0 ; x < 240 ; x += gridWidth )
     {
	  for ( y = 0 ; y < 320 ; y ++ )
	  {
	       lcd_bmp_buffer[y][x] = 0xFFFF;
	  }
     }
     for ( y = 0 ; y < 320 ; y += gridWidth )
     {
	  for ( x = 0 ; x < 240 ; x ++ )
	  {
	       lcd_bmp_buffer[y][x] = 0xFFFF;
	  }
     }
}

void draw_red_frame(int ox, int oy, int ex, int ey)
{
     int x, y;

     y = gridWidth * oy;
     for ( x = gridWidth*ox ; x < gridWidth*(ex) ; x++)
     {
	  lcd_bmp_buffer[y][x] = 0xF800;
     }
     y = gridWidth * (ey);
     for ( x = gridWidth*ox ; x < gridWidth*(ex) ; x++)
     {
	  lcd_bmp_buffer[y][x] = 0xF800;
     }
     x = gridWidth * ox;
     for ( y = gridWidth*oy ; y < gridWidth*(ey) ; y++)
     {
	  lcd_bmp_buffer[y][x] = 0xF800;
     }
     x = gridWidth * (ex);
     for ( y = gridWidth*oy ; y < gridWidth*(ey) ; y++)
     {
	  lcd_bmp_buffer[y][x] = 0xF800;
     }
}

void test_image(void)
{
     int x, y;
     int fntWidth;
     int fntHeight;
     const int height = 10;
     const int width = 30;

     ruint16* fontbuf;
     int byteWidth;

     draw_grid();
     FntSetFont(&FntGlobalContext, FntIDAlpha);

     byteWidth = FntGetByteWidth(&FntGlobalContext);
     fntHeight = FntGetTextHeight(&FntGlobalContext);
     fntWidth = FntGetTextWidth(&FntGlobalContext, "abcdefg", 4);
     fontbuf = FntGetStrImage(&FntGlobalContext, 0, 0, fntWidth, fntHeight, "abcdefg", 4, 0);

     draw_red_frame(0, 0, fntWidth, fntHeight);

     for ( y = 0 ; y < fntHeight ; y++ )
     {
	  for ( x = 0 ; x < fntWidth ; x++ )
	  {
	       if ( fontbuf[ fntWidth*y + x ] ) draw_box(x, y);
	  }
     }
}

static void* rexyThread(void* args)
{
     StartRexy();
}

int main(int argc, char*argv[])
{
     int ret;
     GThread *rexy_thread;

     g_thread_init(NULL);
     gdk_threads_init();

     gtk_init(&argc, &argv);

     build_device();

     RalKernelBuffer = g_async_queue_new();

     if ( !RalKernelBuffer )
     {
	  printf("can't create message buffer\n");
	  return -1;
     }

     rexy_thread = g_thread_create( rexyThread, NULL, TRUE, NULL);

     gdk_threads_enter();
     gtk_main();
     gdk_threads_leave();

     g_thread_join( rexy_thread );
	
     return 0;
}
