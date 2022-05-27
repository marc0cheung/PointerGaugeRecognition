#include <cvirte.h>		
#include <userint.h>
#include <utility.h>
#include <ansi_c.h>
#include <math.h> 
#include "nivision.h"
#include "stdio.h"
#include "pointer_gauge_recognition.h"

static int panelHandle;
static int TabPanelHandle;
static int TabPanelHandle_1;

static Image *SourceImage;
static Image *Threshold_image;
static Image *temp_image;
static Image *Destimage; 
double a;

typedef struct summary
{
	int sum;
	float theat;
}SUMM;

typedef struct coordinate
{
	int x1;
	int y1;
}CODN;

int main (int argc, char *argv[])
{
	if (InitCVIRTE (0, argv, 0) == 0)
		return -1;	/* out of memory */
	if ((panelHandle = LoadPanel (0, "pointer_gauge_recognition.uir", PANEL)) < 0)
		return -1;
	// TODO: Get the panel handle associated with a tab page with GetPanelHandleFromTabPage
	DisplayPanel (panelHandle);
	GetPanelHandleFromTabPage (PANEL, PANEL_SWITCHTABS, 0, &TabPanelHandle);
	GetPanelHandleFromTabPage (PANEL, PANEL_SWITCHTABS, 1, &TabPanelHandle_1);
	SourceImage = imaqCreateImage (IMAQ_IMAGE_U8, 3);
	Threshold_image = imaqCreateImage (IMAQ_IMAGE_U8, 3);
	temp_image = imaqCreateImage (IMAQ_IMAGE_U8, 3);
	RunUserInterface ();
	DiscardPanel (panelHandle);
	return 0;
}

int CVICALLBACK load_image (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int status;
	char filename[512];
	int SrcImg;
	HistogramReport *report;
	switch (event)
	{
		case EVENT_COMMIT:
			status = FileSelectPopup ("", "*.*", "", "Select an Image", VAL_LOAD_BUTTON, 0, 0, 1, 0, filename);
			if(status == 1)
			{
				CanvasClear (TabPanelHandle, TABPANEL_CANVAS, VAL_ENTIRE_OBJECT);
			    GetBitmapFromFile (filename, &SrcImg);
			    CanvasDrawBitmap (TabPanelHandle, TABPANEL_CANVAS, SrcImg,VAL_ENTIRE_OBJECT, VAL_ENTIRE_OBJECT);
			    DiscardBitmap(SrcImg);
				imaqReadFile (SourceImage, filename, NULL, NULL); 
				report=imaqHistogram(SourceImage, 256, 0, 255, IMAQ_IMAGE_U8);
				DeleteGraphPlot (TabPanelHandle, TABPANEL_GRAPH, -1, VAL_IMMEDIATE_DRAW);
                PlotY(TabPanelHandle,TABPANEL_GRAPH,(*report).histogram,256,VAL_UNSIGNED_INTEGER,VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, 1, VAL_RED);
			}
			break;
	}
	return 0;
}

int CVICALLBACK resize_image (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			imaqResample (SourceImage, SourceImage, 320, 240, IMAQ_ZERO_ORDER, IMAQ_NO_RECT);
			imaqSetWindowTitle (1, "Resized Image (320 * 240)");
			imaqMoveWindow (1, imaqMakePoint (100, 100));
			imaqDisplayImage (SourceImage, 1, TRUE);
			break;
	}
	return 0;
}

int CVICALLBACK threshold (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int threshold_val=0;
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal (TabPanelHandle, TABPANEL_THRESHOLD_VALUE, &threshold_val);
			if(threshold_val==0)
			{
			     threshold_val=88;
			}
			imaqThreshold (Threshold_image, SourceImage, threshold_val, 255, TRUE, 255);
			imaqSetWindowTitle (2, "Image After Thresholding");
			imaqMoveWindow (2, imaqMakePoint (300, 100));
			imaqDisplayImage (Threshold_image, 2, TRUE);
			break;
	}
	return 0;
}

int CVICALLBACK reverse_pixel (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	Point point;
	int height;
	int width;
	int gcale_value;
	PixelValue pixelvalue; 
	PixelValue white;
	int x,y;
	
	switch (event)
	{
		case EVENT_COMMIT:
			imaqGetImageSize (Threshold_image, &width, &height);
			for(x = 0; x < width; x ++)
			{
			    for(y = 0; y < height; y ++)
				{
					point.x = x;
					point.y = y;
					imaqGetPixel (Threshold_image, point, &pixelvalue);
					gcale_value = pixelvalue.grayscale;
				    if(gcale_value == 0)
					{
						white.grayscale = 255;
						imaqSetPixel (Threshold_image, point, white);
					}
					else{
					    white.grayscale=0;
						imaqSetPixel (Threshold_image, point, white);
					}
				}
			}
			imaqSetWindowTitle (3, "Image After Reversing Pixels");
			imaqMoveWindow (3, imaqMakePoint (500, 100));
			imaqDisplayImage (Threshold_image, 3, TRUE);
			break;
	}
	return 0;
}

int CVICALLBACK find_line (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int Precision[30]={11,22,33,45,56,69,82,97,106,121,131,142,153,166,179,	 
        189,201,213,225,236,248,260,275,287,300,313,323,335,348,358};
    int Time[30]={1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30};
   
    float length,theat1;
	double t1,testtime,difference;
	ImageInfo image_info;
	Point point;
	PixelValue pixelvalue;
	PixelValue pixelvalue_0;
	PixelValue pixelvalue_1;
	int *fptr;
	int stand;
	double differ,timediff;
	int width,height,x,y,xvalue,yvalue,x1,y1,t,roh,max_roh_theat,sign,k,hough_roh,hough_theat,s,temp,temp1,i,sum1,sum2,rohmax,theat;
	SUMM sumsl[100];
    CODN codn[2000];
	xvalue=159;		   //automatically mark the center point of the gauge
	yvalue=119;
	
	switch (event)
	{
		case EVENT_COMMIT:
			imaqGetImageInfo (Threshold_image, &image_info);
			width=image_info.xRes;
			height=image_info.yRes;
			rohmax=sqrt(width * width + height * height);
			fptr=calloc(rohmax * 400, sizeof(int));	 //Distribute 1D Array to store data
			for(x = 0; x < width; x ++)
			{
			    for(y = 0; y < height; y ++)				 //Create CODN based on the center point of the gauge
				{
				    x1 = x - xvalue;  y1 = y - yvalue;
					length=sqrt(x1 * x1 + y1 * y1);
					if(length > 100)
					{
						point.x = x;
						point.y = y;
						pixelvalue.grayscale = 0;
						imaqSetPixel (Threshold_image, point, pixelvalue);
					}
				}
			}
			imaqSetWindowTitle (4, "Image for Recognition");
			imaqMoveWindow (4, imaqMakePoint (700,100));
			imaqDisplayImage (Threshold_image, 4, TRUE);
			for(x=0;x<width;x++)					 //CODN Transform Calculate every single white point
			{										  
			    for(y=0;y<height;y++)
				{
					point.x=x;
					point.y=y;
					imaqGetPixel (Threshold_image, point, &pixelvalue_0);
					if(pixelvalue_0.grayscale!=0)
					{
					    for(t = 0; t < 180; t ++)					  
		                {
		                     roh=(x*cos(t*3.14159/180)+y*sin(t*3.14159/180)+0.5);
		                     *(fptr+(roh+rohmax)*180+t)=*(fptr+(roh+rohmax)*180+t)+1;
		                }
					}
				}
			}
			max_roh_theat=0;
			sign=0;
			for(roh=-rohmax;roh<rohmax;roh++)			//Find the max value in 2D Array
		    {											//And that's where the line of the pointer locates
		        for(theat=0;theat<180;theat++)
		        {
		            k=*(fptr+(roh+rohmax)*180+theat);
		            if(k>max_roh_theat)
		            {
		                max_roh_theat=k;
		                hough_roh=roh;
		                hough_theat=theat;				 //hough_theat means angles
		            }
		         }
		 	}
			theat1=atan(-cos(hough_theat*3.14159/180)/sin(hough_theat*3.14159/180));   //CODN Transform
			if(theat1>=0)
		 	{			     
				t1=theat1*180/3.14159;
			}
			else
			{
				t1=theat1*180/3.14159+180;
				theat1=3.14159+theat1;
		 	}
			imaqDuplicate (temp_image, SourceImage);
			s=0;
		    for(y=0;y<height;y++)						  //Save the pixels on the line that detected to codn[s]
		    {
		        for(x=0;x<width;x++)
		        {
		            temp=hough_roh-(x*cos(hough_theat*3.14159/180)+y*sin(hough_theat*3.14159/180)+0.5);
		            point.x=x;
					point.y=y;
					if(temp==0)
		            {
						codn[s].x1=x;
						codn[s].y1=y;
						s++;
		            }
		          
				}
			}
			
			sum1=0;   
			sum2=0; 
            for(i=0;i<0.5*s;i++)			  
            {
				point.x=codn[i].x1;
				point.y=codn[i].y1;
				imaqGetPixel (Threshold_image, point, &pixelvalue_1);
                if(pixelvalue_1.grayscale!=0)   sum1++;
            }
            for(i=0.5*s;i<s;i++)
            {
                point.x=codn[i].x1;
				point.y=codn[i].y1;
				imaqGetPixel (Threshold_image, point, &pixelvalue_1);
                if(pixelvalue_1.grayscale!=0)   sum2++;
            }
			if(sum1<sum2)    
			{
				t1+=180;	 
			}
			
		
			t1=t1-90.00;	
			if(t1<=0)
			{
			    t1=360+t1;
			}
			SetCtrlVal(TabPanelHandle,TABPANEL_ANGLE_MS,t1);		
			testtime=t1/360.00*30.00;
			SetCtrlVal(TabPanelHandle,TABPANEL_ANGLE_TIME,testtime);
			
			stand=testtime/1;
			differ=(Precision[stand]-t1)*100/Precision[stand];
			stand=stand+1;
			a=testtime;
			timediff=(stand-a)*100/stand;
			
			
			for(y=0;y<height;y++)				
		    {
		        for(x=0;x<width;x++)
		        {
		            temp1=hough_roh-(x*cos(hough_theat*3.14159/180)+y*sin(hough_theat*3.14159/180)+0.5);
		            point.x=x;
					point.y=y;
					if(temp1==0)
		            {
						pixelvalue.grayscale=0;
		                imaqSetPixel (Threshold_image, point, pixelvalue);
		            }
		            else
		            {
						pixelvalue.grayscale=255;
		                imaqSetPixel (Threshold_image, point, pixelvalue);
					}
				}
			}
			imaqSetWindowTitle (5, "Detected Straight Line");
			imaqMoveWindow (5, imaqMakePoint (900,100));
			imaqDisplayImage (Threshold_image, 5, TRUE);
			SetCtrlVal (TabPanelHandle,TABPANEL_SPEEDOMETER,a);
			SetCtrlVal(TabPanelHandle,TABPANEL_ANGLE_DIFFERENCE,differ);
			SetCtrlVal(TabPanelHandle,TABPANEL_TIMEDIFF,timediff); 
			break;
	}
	return 0;
}

int CVICALLBACK one_press_recognition (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int Precision[30]={11,22,33,45,56,69,82,97,106,121,131,142,153,166,179,
        189,201,213,225,236,248,260,275,287,300,313,323,335,348,358};
	
	Point point;
	int height;
	int width;
	int gcale_value;
	PixelValue pixelvalue; 
	PixelValue white;
	int x,y;
	
	float length,theat1;
	double t1,testtime;
	ImageInfo image_info;
	 int a ;

	PixelValue pixelvalue_0;
	PixelValue pixelvalue_1;
	int *fptr;
	int xvalue,yvalue,x1,y1,t,roh,max_roh_theat,sign,k,hough_roh,hough_theat,s,temp,temp1,i,sum1,sum2,rohmax,theat,num;
	SUMM sumsl[100];
    CODN codn[2000];
	xvalue=159;
	yvalue=119;
	int src,thres;
	int stand;
	double differ;
	
	char Allimagepath[30][512];
	int r=0,h=0;
	
	int key=0;
		
	for(num=0;num<30;num++)
    {
		GetProjectDir (Allimagepath[num]);			  
	}
	strcat(Allimagepath[0],"\\1.bmp");
    strcat(Allimagepath[1],"\\2.bmp");
	strcat(Allimagepath[2],"\\3.bmp");
	strcat(Allimagepath[3],"\\4.bmp");
	strcat(Allimagepath[4],"\\5.bmp");
	strcat(Allimagepath[5],"\\6.bmp");
	strcat(Allimagepath[6],"\\7.bmp");
	strcat(Allimagepath[7],"\\8.bmp");
	strcat(Allimagepath[8],"\\9.bmp");
	strcat(Allimagepath[9],"\\10.bmp");
	strcat(Allimagepath[10],"\\11.bmp");
	strcat(Allimagepath[11],"\\12.bmp");
	strcat(Allimagepath[12],"\\13.bmp");  
	strcat(Allimagepath[13],"\\14.bmp");  
	strcat(Allimagepath[14],"\\15.bmp");  
	strcat(Allimagepath[15],"\\16.bmp");  
	strcat(Allimagepath[16],"\\17.bmp");  
	strcat(Allimagepath[17],"\\18.bmp");  
	strcat(Allimagepath[18],"\\19.bmp");  
	strcat(Allimagepath[19],"\\20.bmp");  
	strcat(Allimagepath[20],"\\21.bmp");  
	strcat(Allimagepath[21],"\\22.bmp");  
	strcat(Allimagepath[22],"\\23.bmp");  
	strcat(Allimagepath[23],"\\24.bmp");  
	strcat(Allimagepath[24],"\\25.bmp");  
	strcat(Allimagepath[25],"\\26.bmp");  
	strcat(Allimagepath[26],"\\27.bmp");  
	strcat(Allimagepath[27],"\\28.bmp");  
	strcat(Allimagepath[28],"\\29.bmp");  
	strcat(Allimagepath[29],"\\30.bmp");	
	
	switch (event)
	{
		case EVENT_COMMIT:
			
		while(h <= 29)
		{
			r = h % 30;
			h ++;
			imaqReadFile (SourceImage, Allimagepath[r], NULL, NULL);
			imaqResample (SourceImage, SourceImage, 320, 240, IMAQ_ZERO_ORDER, IMAQ_NO_RECT);
			imaqWriteFile (SourceImage, "D:\\temp.bmp", NULL);
	        GetBitmapFromFile ("D:\\temp.bmp", &src);
	        CanvasDrawBitmap (TabPanelHandle_1, TABPANEL_2_CANVAS_1, src, VAL_ENTIRE_OBJECT, VAL_ENTIRE_OBJECT);
	        DiscardBitmap(src);
			
			
			imaqThreshold (Threshold_image, SourceImage, 90, 255, TRUE, 255);
			imaqWriteFile (Threshold_image, "D:\\temp.bmp", NULL);
	        GetBitmapFromFile ("D:\\temp.bmp", &thres);
	        CanvasDrawBitmap (TabPanelHandle_1, TABPANEL_2_CANVAS_2, thres, VAL_ENTIRE_OBJECT, VAL_ENTIRE_OBJECT);
	        DiscardBitmap(thres);
			

	
	        imaqGetImageSize (Threshold_image, &width, &height);
			for(x=0;x<width;x++)
			{
			    for(y=0;y<height;y++)
				{
					point.x=x;
					point.y=y;
					imaqGetPixel (Threshold_image, point, &pixelvalue);
					gcale_value=pixelvalue.grayscale;
				    if(gcale_value==0)
					{
						white.grayscale=255;
						imaqSetPixel (Threshold_image, point, white);
					}
					else{
					    white.grayscale=0;
						imaqSetPixel (Threshold_image, point, white);
					}
				}
			}
			imaqWriteFile (Threshold_image, "D:\\temp.bmp", NULL);
	        GetBitmapFromFile ("D:\\temp.bmp", &thres);
	        CanvasDrawBitmap (TabPanelHandle_1, TABPANEL_2_CANVAS_3, thres, VAL_ENTIRE_OBJECT, VAL_ENTIRE_OBJECT);
	        DiscardBitmap(thres);

	        imaqGetImageInfo (Threshold_image, &image_info);
			width=image_info.xRes;
			height=image_info.yRes;
			rohmax=sqrt(width*width+height*height);
			fptr=calloc(rohmax*400,sizeof(int));
			for(x=0;x<width;x++)
			{
			    for(y = 0; y < height; y ++)
				{
				    x1 = x - xvalue;  y1 = y - yvalue;
					length = sqrt(x1 * x1 + y1 * y1);
					if(length > 100)
					{
						point.x = x;
						point.y = y;
						pixelvalue.grayscale = 0;
						imaqSetPixel (Threshold_image, point, pixelvalue);
					}
				}
			}
			imaqWriteFile (Threshold_image, "D:\\temp.bmp", NULL);
	        GetBitmapFromFile ("D:\\temp.bmp", &thres);
	        CanvasDrawBitmap (TabPanelHandle_1, TABPANEL_2_CANVAS_4, thres, VAL_ENTIRE_OBJECT, VAL_ENTIRE_OBJECT);
	        DiscardBitmap(thres);
			for(x = 0; x < width; x ++)
			{
			    for(y = 0; y < height; y ++)
				{
					point.x = x;
					point.y = y;
					imaqGetPixel (Threshold_image, point, &pixelvalue_0);
					if(pixelvalue_0.grayscale != 0)
					{
					    for(t = 0; t < 180; t ++)
		                {
		                     roh = (x * cos(t * 3.14159/180) + y * sin(t * 3.14159/180) + 0.5);
		                     *(fptr+(roh+rohmax)*180+t)=*(fptr+(roh+rohmax)*180+t)+1;
		                }
					}
				}
			}
			max_roh_theat = 0;
			sign = 0;
			
			for(roh=-rohmax; roh < rohmax; roh++)
		    {
		        for(theat = 0; theat < 180; theat ++)
		        {
		            k=*(fptr+(roh+rohmax)*180+theat);
		            if(k>max_roh_theat)
		            {
		                max_roh_theat=k;
		                hough_roh=roh;
		                hough_theat=theat;
		            }
		         }
		 	}
			theat1=atan(-cos(hough_theat*3.14159/180)/sin(hough_theat*3.14159/180));
			if(theat1>=0)
		 	{			     
				t1=theat1*180/3.14159;
			}
			else
			{
				t1=theat1*180/3.14159+180;
				theat1=3.14159+theat1;
		 	}
			imaqDuplicate (temp_image, SourceImage);
			s=0;
		    for(y=0;y<height;y++)
		    {
		        for(x=0;x<width;x++)
		        {
		            temp=hough_roh-(x*cos(hough_theat*3.14159/180)+y*sin(hough_theat*3.14159/180)+0.5);
		            point.x=x;
					point.y=y;
					if(temp==0)
		            {
						pixelvalue.grayscale=0;
						codn[s].x1=x;
						codn[s].y1=y;
						 s++;
		            }
		          
				}
			}
			
			sum1=0;   
			sum2=0; 
            for(i=0;i<0.5*s;i++)
            {
				point.x=codn[i].x1;
				point.y=codn[i].y1;
				imaqGetPixel (Threshold_image, point, &pixelvalue_1);
                if(pixelvalue_1.grayscale!=0)   sum1++;
            }
            for(i = 0.5 * s; i < s; i ++)
            {
                point.x = codn[i].x1;
				point.y = codn[i].y1;
				imaqGetPixel (Threshold_image, point, &pixelvalue_1);
                if(pixelvalue_1.grayscale != 0)   sum2++;
            }
			if(sum1 < sum2)    t1 += 180;
			t1 = t1 - 90.00;
			if(t1 <= 0)
			{
			    t1 = 360 + t1;
			}
			testtime = t1 / 360.00 * 30.00;
			  for(y = 0; y < height; y ++)
		    {
		        for(x = 0; x < width; x ++)
		        {
		            temp1 = hough_roh - (x * cos(hough_theat * 3.14159 / 180) + y * sin(hough_theat * 3.14159/180) + 0.5);
		            point.x = x;
					point.y = y;
					if(temp1 == 0)
		            {
						pixelvalue.grayscale = 0;
		                imaqSetPixel (Threshold_image, point, pixelvalue);
		            }
		            else
		            {
						pixelvalue.grayscale=255;
		                imaqSetPixel (Threshold_image, point, pixelvalue);
					}
				}
			}
			imaqWriteFile (Threshold_image, "D:\\temp.bmp", NULL);
	        GetBitmapFromFile ("D:\\temp.bmp", &thres);
	        CanvasDrawBitmap (TabPanelHandle_1, TABPANEL_2_CANVAS_5, thres, VAL_ENTIRE_OBJECT, VAL_ENTIRE_OBJECT);
	        DiscardBitmap(thres);		
			a = testtime / 1;
			stand = a/1 + 1;
			differ = (Precision[a] - t1) * 100 / Precision[a];
			SetCtrlVal (TabPanelHandle_1, TABPANEL_2_SPEEDOMETER_2,testtime);
			SetCtrlVal(TabPanelHandle_1,TABPANEL_2_ANGLE_DIFF,stand);
		}
	}
	return 0;
}



int CVICALLBACK quit (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			QuitUserInterface (0);
			break;
	}
	return 0;
}

int CVICALLBACK quit2 (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			QuitUserInterface (0);
			break;
	}
	return 0;
}


int CVICALLBACK quit3 (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			QuitUserInterface (0);
			break;
	}
	return 0;
}
