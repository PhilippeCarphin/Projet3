#define WHERE xil_printf("%s():",__FUNCTION__);
#define WHEREL xil_printf("%s() (%s,%d)",__FUNCTION__,__FILE__,__LINE__);

#define FBEGIN xil_printf("%s(): Begin\n",__FUNCTION__);
#define FEND xil_printf("%s(): End\n",__FUNCTION__);
