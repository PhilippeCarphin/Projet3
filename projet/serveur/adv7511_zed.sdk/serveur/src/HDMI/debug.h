

#ifdef DEBUG
	#define WHERE xil_printf("%s():",__FUNCTION__);
	#define WHEREL xil_printf("%s() (%s,%d)",__FUNCTION__,__FILE__,__LINE__);
	#define FBEGIN xil_printf("%s(): Begin\n",__FUNCTION__);
	#define FEND xil_printf("%s(): End\n",__FUNCTION__);
	#define DBG_PRINT(...) xil_printf(__VA_ARGS__)
#else
	#define WHERE
	#define WHEREL
	#define FBEGIN
	#define FEND
	#define DBG_PRINT(...) do {} while (0)
#endif
