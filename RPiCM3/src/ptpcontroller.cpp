#include <ptpcontroller.h>

// Code from this file is taken from the libptp2 ptpcam program,
// and uses the libptp2 library.
#include <libptp2/ptp.h>

void PTPController::getProperty(uint16_t propertyCode)
{
    getset_property(busn,devn,propertyCode,0,force);
}

void PTPController::setProperty(uint16_t propertyCode, char *value)
{
    getset_property(busn,devn,propertyCode,value,force);
}























































// #include <errno.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <unistd.h>
// #include <signal.h>
// #include <sys/types.h>
// #include <utime.h>
// #include <sys/stat.h>
// #include <fcntl.h>
// #include <sys/mman.h>
// #include <usb.h>

// #define USB_BULK_READ usb_bulk_read
// #define USB_BULK_WRITE usb_bulk_write

// /* Check value and Return on error */
// #define CR(o,error) {						\
// 			uint16_t result=o;				\
// 			if((result)!=PTP_RC_OK) {			\
// 				ptp_perror(&params,result);		\
// 				fprintf(stderr,"ERROR: "error);		\
// 				close_camera(&ptp_usb, &params, dev);   \
// 				return;					\
// 			}						\
// }

// /* Check value and Continue on error */
// #define CC(result,error) {						\
// 			if((result)!=PTP_RC_OK) {			\
// 				fprintf(stderr,"ERROR: "error);		\
// 				usb_release_interface(ptp_usb.handle,	\
// 		dev->config->interface->altsetting->bInterfaceNumber);	\
// 				continue;					\
// 			}						\
// }

// /* error reporting macro */
// #define ERROR(error) fprintf(stderr,"ERROR: "error);				

// /* property value printing macros */
// #define PRINT_PROPVAL_DEC(value)	\
// 		print_propval(dpd.DataType, value,			\
// 		PTPCAM_PRINT_DEC)

// #define PRINT_PROPVAL_HEX(value)					\
// 		print_propval(dpd.DataType, value,			\
// 		PTPCAM_PRINT_HEX)

// /* requested actions */
// #define ACT_DEVICE_RESET	0x1
// #define ACT_LIST_DEVICES	0x2
// #define ACT_LIST_PROPERTIES	0x3
// #define ACT_LIST_OPERATIONS	0x4
// #define ACT_GETSET_PROPERTY	0x5
// #define ACT_SHOW_INFO		0x6
// #define ACT_LIST_FILES		0x7
// #define ACT_GET_FILE		0x8
// #define ACT_GET_ALL_FILES	0x9
// #define ACT_CAPTURE		0xA
// #define ACT_DELETE_OBJECT	0xB
// #define ACT_DELETE_ALL_FILES	0xC
// #define ACT_LOOP_CAPTURE	0xD
// #define ACT_SHOW_ALL_PROPERTIES	0xE
// #define ACT_SHOW_UNKNOWN_PROPERTIES	0xF
// #define ACT_SET_PROPBYNAME	0x10
// #define ACT_GENERIC_REQ     0x11

// #define ACT_NIKON_DC		0x101
// #define ACT_NIKON_DC2		0x102
// #define ACT_NIKON_IC		0x103

// /* printing value type */
// #define PTPCAM_PRINT_HEX	00
// #define PTPCAM_PRINT_DEC	01

// /* filename overwrite */
// #define OVERWRITE_EXISTING	1
// #define	SKIP_IF_EXISTS		0

// #define textdomain(String) (String)
// #define gettext(String) (String)
// #define dgettext(Domain,Message) (Message)
// #define dcgettext(Domain,Message,Type) (Message)
// #define bindtextdomain(Domain,Directory) (Domain)
// #define _(String) (String)
// #define N_(String) (String)

// /* USB interface class */
// #ifndef USB_CLASS_PTP
// #define USB_CLASS_PTP		6
// #endif

// /* USB control message data phase direction */
// #ifndef USB_DP_HTD
// #define USB_DP_HTD		(0x00 << 7)	/* host to device */
// #endif
// #ifndef USB_DP_DTH
// #define USB_DP_DTH		(0x01 << 7)	/* device to host */
// #endif

// /* PTP class specific requests */
// #ifndef USB_REQ_DEVICE_RESET
// #define USB_REQ_DEVICE_RESET		0x66
// #endif
// #ifndef USB_REQ_GET_DEVICE_STATUS
// #define USB_REQ_GET_DEVICE_STATUS	0x67
// #endif

// /* USB Feature selector HALT */
// #ifndef USB_FEATURE_HALT
// #define USB_FEATURE_HALT	0x00
// #endif

// /* OUR APPLICATION USB URB (2MB) ;) */
// #define PTPCAM_USB_URB		2097152

// #define USB_TIMEOUT		5000
// #define USB_CAPTURE_TIMEOUT	20000

// /* the other one, it sucks definitely ;) */
// int ptpcam_usb_timeout = USB_TIMEOUT;

// /* we need it for a proper signal handling :/ */
// PTPParams* globalparams;

// static short PTPController::ptp_read_func (unsigned char *bytes, unsigned int size, void *data)
// {
// 	int result=-1;
// 	PTP_USB *ptp_usb=(PTP_USB *)data;
// 	int toread=0;
// 	signed long int rbytes=size;

// 	do {
// 		bytes+=toread;
// 		if (rbytes>PTPCAM_USB_URB) 
// 			toread = PTPCAM_USB_URB;
// 		else
// 			toread = rbytes;
// 		result=USB_BULK_READ(ptp_usb->handle, ptp_usb->inep,(char *)bytes, toread,ptpcam_usb_timeout);
// 		/* sometimes retry might help */
// 		if (result==0)
// 			result=USB_BULK_READ(ptp_usb->handle, ptp_usb->inep,(char *)bytes, toread,ptpcam_usb_timeout);
// 		if (result < 0)
// 			break;
// 		rbytes-=PTPCAM_USB_URB;
// 	} while (rbytes>0);

// 	if (result >= 0) {
// 		return (PTP_RC_OK);
// 	}
// 	else 
// 	{
// 		if (verbose) perror("usb_bulk_read");
// 		return PTP_ERROR_IO;
// 	}
// }

// static short ptp_write_func (unsigned char *bytes, unsigned int size, void *data)
// {
// 	int result;
// 	PTP_USB *ptp_usb=(PTP_USB *)data;

// 	result=USB_BULK_WRITE(ptp_usb->handle,ptp_usb->outep,(char *)bytes,size,ptpcam_usb_timeout);
// 	if (result >= 0)
// 		return (PTP_RC_OK);
// 	else 
// 	{
// 		if (verbose) perror("usb_bulk_write");
// 		return PTP_ERROR_IO;
// 	}
// }

// /* XXX this one is suposed to return the number of bytes read!!! */
// static short ptp_check_int (unsigned char *bytes, unsigned int size, void *data)
// {
// 	int result;
// 	PTP_USB *ptp_usb=(PTP_USB *)data;

// 	result=USB_BULK_READ(ptp_usb->handle, ptp_usb->intep,(char *)bytes,size,ptpcam_usb_timeout);
// 	if (result==0)
// 	    result=USB_BULK_READ(ptp_usb->handle, ptp_usb->intep,(char *)bytes,size,ptpcam_usb_timeout);
// 	if (verbose>2) fprintf (stderr, "USB_BULK_READ returned %i, size=%i\n", result, size);

// 	if (result >= 0) {
// 		return result;
// 	} else {
// 		if (verbose) perror("ptp_check_int");
// 		return result;
// 	}
// }