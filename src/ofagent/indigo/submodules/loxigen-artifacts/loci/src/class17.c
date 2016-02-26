/* Copyright (c) 2008 The Board of Trustees of The Leland Stanford Junior University */
/* Copyright (c) 2011, 2012 Open Networking Foundation */
/* Copyright (c) 2012, 2013 Big Switch Networks, Inc. */
/* See the file LICENSE.loci which should have been included in the source distribution */
#ifdef __GNUC__

#ifdef __linux__
/* glibc */
#include <features.h>
#else
/* NetBSD etc */
#include <sys/cdefs.h>
#ifdef __GNUC_PREREQ__
#define __GNUC_PREREQ __GNUC_PREREQ__
#endif
#endif

#ifndef __GNUC_PREREQ
/* fallback */
#define __GNUC_PREREQ(maj, min) 0
#endif

#if __GNUC_PREREQ(4,6)
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif

#endif

#include "loci_log.h"
#include "loci_int.h"

static void
ofdpa_color_based_ctr_multipart_request_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint8_t *)(buf + 0) = obj->version; /* version */
        *(uint8_t *)(buf + 1) = 0x12; /* type */
        *(uint32_t *)(buf + 8) = U32_HTON(0x1018); /* experimenter */
        *(uint32_t *)(buf + 12) = U32_HTON(0xf); /* subtype */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup ofdpa_color_based_ctr_multipart_request ofdpa_color_based_ctr_multipart_request
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
ofdpa_color_based_ctr_multipart_request_push_wire_values(ofdpa_color_based_ctr_multipart_request_t *obj)
{

    ofdpa_color_based_ctr_multipart_request_push_wire_types(obj);

    /* Message obj; set length */
    of_message_t msg;

    if ((msg = OF_OBJECT_TO_MESSAGE(obj)) != NULL) {
        of_message_length_set(msg, obj->length);
    }

    return OF_ERROR_NONE;
}

/**
 * Create a new ofdpa_color_based_ctr_multipart_request object
 *
 * @param version The wire version to use for the object
 * @return Pointer to the newly create object or NULL on error
 *
 * Initializes the new object with it's default fixed length associating
 * a new underlying wire buffer.
 *
 * Use new_from_message to bind an existing message to a message object,
 * or a _get function for non-message objects.
 *
 * \ingroup ofdpa_color_based_ctr_multipart_request
 */

ofdpa_color_based_ctr_multipart_request_t *
ofdpa_color_based_ctr_multipart_request_new(of_version_t version)
{
    ofdpa_color_based_ctr_multipart_request_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OFDPA_COLOR_BASED_CTR_MULTIPART_REQUEST] + of_object_extra_len[version][OFDPA_COLOR_BASED_CTR_MULTIPART_REQUEST];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (ofdpa_color_based_ctr_multipart_request_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    ofdpa_color_based_ctr_multipart_request_init(obj, version, bytes, 0);

    if (ofdpa_color_based_ctr_multipart_request_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type ofdpa_color_based_ctr_multipart_request.
 *
 * @param obj Pointer to the object to initialize
 * @param version The wire version to use for the object
 * @param bytes How many bytes in the object
 * @param clean_wire Boolean: If true, clear the wire object control struct
 *
 * If bytes < 0, then the default fixed length is used for the object
 *
 * This is a "coerce" function that sets up the pointers for the
 * accessors properly.
 *
 * If anything other than 0 is passed in for the buffer size, the underlying
 * wire buffer will have 'grow' called.
 */

void
ofdpa_color_based_ctr_multipart_request_init(ofdpa_color_based_ctr_multipart_request_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OFDPA_COLOR_BASED_CTR_MULTIPART_REQUEST] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OFDPA_COLOR_BASED_CTR_MULTIPART_REQUEST] + of_object_extra_len[version][OFDPA_COLOR_BASED_CTR_MULTIPART_REQUEST];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OFDPA_COLOR_BASED_CTR_MULTIPART_REQUEST;

    /* Set up the object's function pointers */

    obj->wire_type_set = ofdpa_color_based_ctr_multipart_request_push_wire_types;

    obj->wire_length_get = of_object_message_wire_length_get;
    obj->wire_length_set = of_object_message_wire_length_set;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Create a new ofdpa_color_based_ctr_multipart_request object and bind it to an existing message
 *
 * @param msg The message to bind the new object to
 * @return Pointer to the newly create object or NULL on error
 *
 * \ingroup ofdpa_color_based_ctr_multipart_request
 */

ofdpa_color_based_ctr_multipart_request_t *
ofdpa_color_based_ctr_multipart_request_new_from_message(of_message_t msg)
{
    ofdpa_color_based_ctr_multipart_request_t *obj = NULL;
    of_version_t version;
    int length;

    if (msg == NULL) return NULL;

    version = of_message_version_get(msg);
    if (!OF_VERSION_OKAY(version)) return NULL;

    length = of_message_length_get(msg);

    if ((obj = (ofdpa_color_based_ctr_multipart_request_t *)of_object_new(-1)) == NULL) {
        return NULL;
    }

    ofdpa_color_based_ctr_multipart_request_init(obj, version, 0, 0);

    if ((of_object_buffer_bind((of_object_t *)obj, OF_MESSAGE_TO_BUFFER(msg),
                               length, OF_MESSAGE_FREE_FUNCTION)) < 0) {
       FREE(obj);
       return NULL;
    }
    obj->length = length;
    obj->version = version;

    return obj;
}

/**
 * Get xid from an object of type ofdpa_color_based_ctr_multipart_request.
 * @param obj Pointer to an object of type ofdpa_color_based_ctr_multipart_request.
 * @param xid Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
ofdpa_color_based_ctr_multipart_request_xid_get(
    ofdpa_color_based_ctr_multipart_request_t *obj,
    uint32_t *xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_COLOR_BASED_CTR_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, xid);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set xid in an object of type ofdpa_color_based_ctr_multipart_request.
 * @param obj Pointer to an object of type ofdpa_color_based_ctr_multipart_request.
 * @param xid The value to write into the object
 */
void
ofdpa_color_based_ctr_multipart_request_xid_set(
    ofdpa_color_based_ctr_multipart_request_t *obj,
    uint32_t xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_COLOR_BASED_CTR_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, xid);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get experimenter from an object of type ofdpa_color_based_ctr_multipart_request.
 * @param obj Pointer to an object of type ofdpa_color_based_ctr_multipart_request.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
ofdpa_color_based_ctr_multipart_request_experimenter_get(
    ofdpa_color_based_ctr_multipart_request_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_COLOR_BASED_CTR_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 8;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set experimenter in an object of type ofdpa_color_based_ctr_multipart_request.
 * @param obj Pointer to an object of type ofdpa_color_based_ctr_multipart_request.
 * @param experimenter The value to write into the object
 */
void
ofdpa_color_based_ctr_multipart_request_experimenter_set(
    ofdpa_color_based_ctr_multipart_request_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_COLOR_BASED_CTR_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 8;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get subtype from an object of type ofdpa_color_based_ctr_multipart_request.
 * @param obj Pointer to an object of type ofdpa_color_based_ctr_multipart_request.
 * @param subtype Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
ofdpa_color_based_ctr_multipart_request_subtype_get(
    ofdpa_color_based_ctr_multipart_request_t *obj,
    uint32_t *subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_COLOR_BASED_CTR_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 12;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set subtype in an object of type ofdpa_color_based_ctr_multipart_request.
 * @param obj Pointer to an object of type ofdpa_color_based_ctr_multipart_request.
 * @param subtype The value to write into the object
 */
void
ofdpa_color_based_ctr_multipart_request_subtype_set(
    ofdpa_color_based_ctr_multipart_request_t *obj,
    uint32_t subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_COLOR_BASED_CTR_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 12;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get block_index from an object of type ofdpa_color_based_ctr_multipart_request.
 * @param obj Pointer to an object of type ofdpa_color_based_ctr_multipart_request.
 * @param block_index Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
ofdpa_color_based_ctr_multipart_request_block_index_get(
    ofdpa_color_based_ctr_multipart_request_t *obj,
    uint32_t *block_index)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_COLOR_BASED_CTR_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 16;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, block_index);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set block_index in an object of type ofdpa_color_based_ctr_multipart_request.
 * @param obj Pointer to an object of type ofdpa_color_based_ctr_multipart_request.
 * @param block_index The value to write into the object
 */
void
ofdpa_color_based_ctr_multipart_request_block_index_set(
    ofdpa_color_based_ctr_multipart_request_t *obj,
    uint32_t block_index)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_COLOR_BASED_CTR_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 16;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, block_index);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get transmit_packets from an object of type ofdpa_color_based_ctr_multipart_request.
 * @param obj Pointer to an object of type ofdpa_color_based_ctr_multipart_request.
 * @param transmit_packets Pointer to the child object of type
 * uint64_t to be filled out.
 *
 */
void
ofdpa_color_based_ctr_multipart_request_transmit_packets_get(
    ofdpa_color_based_ctr_multipart_request_t *obj,
    uint64_t *transmit_packets)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_COLOR_BASED_CTR_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 20;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u64_get(wbuf, abs_offset, transmit_packets);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set transmit_packets in an object of type ofdpa_color_based_ctr_multipart_request.
 * @param obj Pointer to an object of type ofdpa_color_based_ctr_multipart_request.
 * @param transmit_packets The value to write into the object
 */
void
ofdpa_color_based_ctr_multipart_request_transmit_packets_set(
    ofdpa_color_based_ctr_multipart_request_t *obj,
    uint64_t transmit_packets)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_COLOR_BASED_CTR_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 20;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u64_set(wbuf, abs_offset, transmit_packets);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get receive_packets from an object of type ofdpa_color_based_ctr_multipart_request.
 * @param obj Pointer to an object of type ofdpa_color_based_ctr_multipart_request.
 * @param receive_packets Pointer to the child object of type
 * uint64_t to be filled out.
 *
 */
void
ofdpa_color_based_ctr_multipart_request_receive_packets_get(
    ofdpa_color_based_ctr_multipart_request_t *obj,
    uint64_t *receive_packets)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_COLOR_BASED_CTR_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 28;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u64_get(wbuf, abs_offset, receive_packets);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set receive_packets in an object of type ofdpa_color_based_ctr_multipart_request.
 * @param obj Pointer to an object of type ofdpa_color_based_ctr_multipart_request.
 * @param receive_packets The value to write into the object
 */
void
ofdpa_color_based_ctr_multipart_request_receive_packets_set(
    ofdpa_color_based_ctr_multipart_request_t *obj,
    uint64_t receive_packets)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_COLOR_BASED_CTR_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 28;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u64_set(wbuf, abs_offset, receive_packets);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get color from an object of type ofdpa_color_based_ctr_multipart_request.
 * @param obj Pointer to an object of type ofdpa_color_based_ctr_multipart_request.
 * @param color Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
ofdpa_color_based_ctr_multipart_request_color_get(
    ofdpa_color_based_ctr_multipart_request_t *obj,
    uint8_t *color)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_COLOR_BASED_CTR_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 36;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_get(wbuf, abs_offset, color);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set color in an object of type ofdpa_color_based_ctr_multipart_request.
 * @param obj Pointer to an object of type ofdpa_color_based_ctr_multipart_request.
 * @param color The value to write into the object
 */
void
ofdpa_color_based_ctr_multipart_request_color_set(
    ofdpa_color_based_ctr_multipart_request_t *obj,
    uint8_t color)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_COLOR_BASED_CTR_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 36;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_set(wbuf, abs_offset, color);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}
/* Copyright (c) 2008 The Board of Trustees of The Leland Stanford Junior University */
/* Copyright (c) 2011, 2012 Open Networking Foundation */
/* Copyright (c) 2012, 2013 Big Switch Networks, Inc. */
/* See the file LICENSE.loci which should have been included in the source distribution */
#ifdef __GNUC__

#ifdef __linux__
/* glibc */
#include <features.h>
#else
/* NetBSD etc */
#include <sys/cdefs.h>
#ifdef __GNUC_PREREQ__
#define __GNUC_PREREQ __GNUC_PREREQ__
#endif
#endif

#ifndef __GNUC_PREREQ
/* fallback */
#define __GNUC_PREREQ(maj, min) 0
#endif

#if __GNUC_PREREQ(4,6)
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif

#endif

#include "loci_log.h"
#include "loci_int.h"

static void
ofdpa_l2_interface_remark_action_mod_msg_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint8_t *)(buf + 0) = obj->version; /* version */
        *(uint8_t *)(buf + 1) = 0x4; /* type */
        *(uint32_t *)(buf + 8) = U32_HTON(0x1018); /* experimenter */
        *(uint32_t *)(buf + 12) = U32_HTON(0xa); /* subtype */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup ofdpa_l2_interface_remark_action_mod_msg ofdpa_l2_interface_remark_action_mod_msg
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
ofdpa_l2_interface_remark_action_mod_msg_push_wire_values(ofdpa_l2_interface_remark_action_mod_msg_t *obj)
{

    ofdpa_l2_interface_remark_action_mod_msg_push_wire_types(obj);

    /* Message obj; set length */
    of_message_t msg;

    if ((msg = OF_OBJECT_TO_MESSAGE(obj)) != NULL) {
        of_message_length_set(msg, obj->length);
    }

    return OF_ERROR_NONE;
}

/**
 * Create a new ofdpa_l2_interface_remark_action_mod_msg object
 *
 * @param version The wire version to use for the object
 * @return Pointer to the newly create object or NULL on error
 *
 * Initializes the new object with it's default fixed length associating
 * a new underlying wire buffer.
 *
 * Use new_from_message to bind an existing message to a message object,
 * or a _get function for non-message objects.
 *
 * \ingroup ofdpa_l2_interface_remark_action_mod_msg
 */

ofdpa_l2_interface_remark_action_mod_msg_t *
ofdpa_l2_interface_remark_action_mod_msg_new(of_version_t version)
{
    ofdpa_l2_interface_remark_action_mod_msg_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OFDPA_L2_INTERFACE_REMARK_ACTION_MOD_MSG] + of_object_extra_len[version][OFDPA_L2_INTERFACE_REMARK_ACTION_MOD_MSG];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (ofdpa_l2_interface_remark_action_mod_msg_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    ofdpa_l2_interface_remark_action_mod_msg_init(obj, version, bytes, 0);

    if (ofdpa_l2_interface_remark_action_mod_msg_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type ofdpa_l2_interface_remark_action_mod_msg.
 *
 * @param obj Pointer to the object to initialize
 * @param version The wire version to use for the object
 * @param bytes How many bytes in the object
 * @param clean_wire Boolean: If true, clear the wire object control struct
 *
 * If bytes < 0, then the default fixed length is used for the object
 *
 * This is a "coerce" function that sets up the pointers for the
 * accessors properly.
 *
 * If anything other than 0 is passed in for the buffer size, the underlying
 * wire buffer will have 'grow' called.
 */

void
ofdpa_l2_interface_remark_action_mod_msg_init(ofdpa_l2_interface_remark_action_mod_msg_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OFDPA_L2_INTERFACE_REMARK_ACTION_MOD_MSG] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OFDPA_L2_INTERFACE_REMARK_ACTION_MOD_MSG] + of_object_extra_len[version][OFDPA_L2_INTERFACE_REMARK_ACTION_MOD_MSG];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OFDPA_L2_INTERFACE_REMARK_ACTION_MOD_MSG;

    /* Set up the object's function pointers */

    obj->wire_type_set = ofdpa_l2_interface_remark_action_mod_msg_push_wire_types;

    obj->wire_length_get = of_object_message_wire_length_get;
    obj->wire_length_set = of_object_message_wire_length_set;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Create a new ofdpa_l2_interface_remark_action_mod_msg object and bind it to an existing message
 *
 * @param msg The message to bind the new object to
 * @return Pointer to the newly create object or NULL on error
 *
 * \ingroup ofdpa_l2_interface_remark_action_mod_msg
 */

ofdpa_l2_interface_remark_action_mod_msg_t *
ofdpa_l2_interface_remark_action_mod_msg_new_from_message(of_message_t msg)
{
    ofdpa_l2_interface_remark_action_mod_msg_t *obj = NULL;
    of_version_t version;
    int length;

    if (msg == NULL) return NULL;

    version = of_message_version_get(msg);
    if (!OF_VERSION_OKAY(version)) return NULL;

    length = of_message_length_get(msg);

    if ((obj = (ofdpa_l2_interface_remark_action_mod_msg_t *)of_object_new(-1)) == NULL) {
        return NULL;
    }

    ofdpa_l2_interface_remark_action_mod_msg_init(obj, version, 0, 0);

    if ((of_object_buffer_bind((of_object_t *)obj, OF_MESSAGE_TO_BUFFER(msg),
                               length, OF_MESSAGE_FREE_FUNCTION)) < 0) {
       FREE(obj);
       return NULL;
    }
    obj->length = length;
    obj->version = version;

    return obj;
}

/**
 * Get xid from an object of type ofdpa_l2_interface_remark_action_mod_msg.
 * @param obj Pointer to an object of type ofdpa_l2_interface_remark_action_mod_msg.
 * @param xid Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
ofdpa_l2_interface_remark_action_mod_msg_xid_get(
    ofdpa_l2_interface_remark_action_mod_msg_t *obj,
    uint32_t *xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_L2_INTERFACE_REMARK_ACTION_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, xid);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set xid in an object of type ofdpa_l2_interface_remark_action_mod_msg.
 * @param obj Pointer to an object of type ofdpa_l2_interface_remark_action_mod_msg.
 * @param xid The value to write into the object
 */
void
ofdpa_l2_interface_remark_action_mod_msg_xid_set(
    ofdpa_l2_interface_remark_action_mod_msg_t *obj,
    uint32_t xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_L2_INTERFACE_REMARK_ACTION_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, xid);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get experimenter from an object of type ofdpa_l2_interface_remark_action_mod_msg.
 * @param obj Pointer to an object of type ofdpa_l2_interface_remark_action_mod_msg.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
ofdpa_l2_interface_remark_action_mod_msg_experimenter_get(
    ofdpa_l2_interface_remark_action_mod_msg_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_L2_INTERFACE_REMARK_ACTION_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 8;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set experimenter in an object of type ofdpa_l2_interface_remark_action_mod_msg.
 * @param obj Pointer to an object of type ofdpa_l2_interface_remark_action_mod_msg.
 * @param experimenter The value to write into the object
 */
void
ofdpa_l2_interface_remark_action_mod_msg_experimenter_set(
    ofdpa_l2_interface_remark_action_mod_msg_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_L2_INTERFACE_REMARK_ACTION_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 8;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get subtype from an object of type ofdpa_l2_interface_remark_action_mod_msg.
 * @param obj Pointer to an object of type ofdpa_l2_interface_remark_action_mod_msg.
 * @param subtype Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
ofdpa_l2_interface_remark_action_mod_msg_subtype_get(
    ofdpa_l2_interface_remark_action_mod_msg_t *obj,
    uint32_t *subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_L2_INTERFACE_REMARK_ACTION_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 12;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set subtype in an object of type ofdpa_l2_interface_remark_action_mod_msg.
 * @param obj Pointer to an object of type ofdpa_l2_interface_remark_action_mod_msg.
 * @param subtype The value to write into the object
 */
void
ofdpa_l2_interface_remark_action_mod_msg_subtype_set(
    ofdpa_l2_interface_remark_action_mod_msg_t *obj,
    uint32_t subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_L2_INTERFACE_REMARK_ACTION_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 12;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get mod_command from an object of type ofdpa_l2_interface_remark_action_mod_msg.
 * @param obj Pointer to an object of type ofdpa_l2_interface_remark_action_mod_msg.
 * @param mod_command Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
ofdpa_l2_interface_remark_action_mod_msg_mod_command_get(
    ofdpa_l2_interface_remark_action_mod_msg_t *obj,
    uint32_t *mod_command)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_L2_INTERFACE_REMARK_ACTION_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 16;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, mod_command);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set mod_command in an object of type ofdpa_l2_interface_remark_action_mod_msg.
 * @param obj Pointer to an object of type ofdpa_l2_interface_remark_action_mod_msg.
 * @param mod_command The value to write into the object
 */
void
ofdpa_l2_interface_remark_action_mod_msg_mod_command_set(
    ofdpa_l2_interface_remark_action_mod_msg_t *obj,
    uint32_t mod_command)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_L2_INTERFACE_REMARK_ACTION_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 16;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, mod_command);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get index from an object of type ofdpa_l2_interface_remark_action_mod_msg.
 * @param obj Pointer to an object of type ofdpa_l2_interface_remark_action_mod_msg.
 * @param index Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
ofdpa_l2_interface_remark_action_mod_msg_index_get(
    ofdpa_l2_interface_remark_action_mod_msg_t *obj,
    uint32_t *index)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_L2_INTERFACE_REMARK_ACTION_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 20;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, index);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set index in an object of type ofdpa_l2_interface_remark_action_mod_msg.
 * @param obj Pointer to an object of type ofdpa_l2_interface_remark_action_mod_msg.
 * @param index The value to write into the object
 */
void
ofdpa_l2_interface_remark_action_mod_msg_index_set(
    ofdpa_l2_interface_remark_action_mod_msg_t *obj,
    uint32_t index)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_L2_INTERFACE_REMARK_ACTION_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 20;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, index);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get traffic_class from an object of type ofdpa_l2_interface_remark_action_mod_msg.
 * @param obj Pointer to an object of type ofdpa_l2_interface_remark_action_mod_msg.
 * @param traffic_class Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
ofdpa_l2_interface_remark_action_mod_msg_traffic_class_get(
    ofdpa_l2_interface_remark_action_mod_msg_t *obj,
    uint8_t *traffic_class)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_L2_INTERFACE_REMARK_ACTION_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 24;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_get(wbuf, abs_offset, traffic_class);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set traffic_class in an object of type ofdpa_l2_interface_remark_action_mod_msg.
 * @param obj Pointer to an object of type ofdpa_l2_interface_remark_action_mod_msg.
 * @param traffic_class The value to write into the object
 */
void
ofdpa_l2_interface_remark_action_mod_msg_traffic_class_set(
    ofdpa_l2_interface_remark_action_mod_msg_t *obj,
    uint8_t traffic_class)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_L2_INTERFACE_REMARK_ACTION_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 24;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_set(wbuf, abs_offset, traffic_class);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get color from an object of type ofdpa_l2_interface_remark_action_mod_msg.
 * @param obj Pointer to an object of type ofdpa_l2_interface_remark_action_mod_msg.
 * @param color Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
ofdpa_l2_interface_remark_action_mod_msg_color_get(
    ofdpa_l2_interface_remark_action_mod_msg_t *obj,
    uint8_t *color)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_L2_INTERFACE_REMARK_ACTION_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 25;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_get(wbuf, abs_offset, color);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set color in an object of type ofdpa_l2_interface_remark_action_mod_msg.
 * @param obj Pointer to an object of type ofdpa_l2_interface_remark_action_mod_msg.
 * @param color The value to write into the object
 */
void
ofdpa_l2_interface_remark_action_mod_msg_color_set(
    ofdpa_l2_interface_remark_action_mod_msg_t *obj,
    uint8_t color)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_L2_INTERFACE_REMARK_ACTION_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 25;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_set(wbuf, abs_offset, color);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get dscp from an object of type ofdpa_l2_interface_remark_action_mod_msg.
 * @param obj Pointer to an object of type ofdpa_l2_interface_remark_action_mod_msg.
 * @param dscp Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
ofdpa_l2_interface_remark_action_mod_msg_dscp_get(
    ofdpa_l2_interface_remark_action_mod_msg_t *obj,
    uint8_t *dscp)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_L2_INTERFACE_REMARK_ACTION_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 26;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_get(wbuf, abs_offset, dscp);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set dscp in an object of type ofdpa_l2_interface_remark_action_mod_msg.
 * @param obj Pointer to an object of type ofdpa_l2_interface_remark_action_mod_msg.
 * @param dscp The value to write into the object
 */
void
ofdpa_l2_interface_remark_action_mod_msg_dscp_set(
    ofdpa_l2_interface_remark_action_mod_msg_t *obj,
    uint8_t dscp)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_L2_INTERFACE_REMARK_ACTION_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 26;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_set(wbuf, abs_offset, dscp);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get vlan_pcp from an object of type ofdpa_l2_interface_remark_action_mod_msg.
 * @param obj Pointer to an object of type ofdpa_l2_interface_remark_action_mod_msg.
 * @param vlan_pcp Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
ofdpa_l2_interface_remark_action_mod_msg_vlan_pcp_get(
    ofdpa_l2_interface_remark_action_mod_msg_t *obj,
    uint8_t *vlan_pcp)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_L2_INTERFACE_REMARK_ACTION_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 27;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_get(wbuf, abs_offset, vlan_pcp);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set vlan_pcp in an object of type ofdpa_l2_interface_remark_action_mod_msg.
 * @param obj Pointer to an object of type ofdpa_l2_interface_remark_action_mod_msg.
 * @param vlan_pcp The value to write into the object
 */
void
ofdpa_l2_interface_remark_action_mod_msg_vlan_pcp_set(
    ofdpa_l2_interface_remark_action_mod_msg_t *obj,
    uint8_t vlan_pcp)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_L2_INTERFACE_REMARK_ACTION_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 27;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_set(wbuf, abs_offset, vlan_pcp);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get vlan_dei from an object of type ofdpa_l2_interface_remark_action_mod_msg.
 * @param obj Pointer to an object of type ofdpa_l2_interface_remark_action_mod_msg.
 * @param vlan_dei Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
ofdpa_l2_interface_remark_action_mod_msg_vlan_dei_get(
    ofdpa_l2_interface_remark_action_mod_msg_t *obj,
    uint8_t *vlan_dei)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_L2_INTERFACE_REMARK_ACTION_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 28;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_get(wbuf, abs_offset, vlan_dei);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set vlan_dei in an object of type ofdpa_l2_interface_remark_action_mod_msg.
 * @param obj Pointer to an object of type ofdpa_l2_interface_remark_action_mod_msg.
 * @param vlan_dei The value to write into the object
 */
void
ofdpa_l2_interface_remark_action_mod_msg_vlan_dei_set(
    ofdpa_l2_interface_remark_action_mod_msg_t *obj,
    uint8_t vlan_dei)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_L2_INTERFACE_REMARK_ACTION_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 28;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_set(wbuf, abs_offset, vlan_dei);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}
/* Copyright (c) 2008 The Board of Trustees of The Leland Stanford Junior University */
/* Copyright (c) 2011, 2012 Open Networking Foundation */
/* Copyright (c) 2012, 2013 Big Switch Networks, Inc. */
/* See the file LICENSE.loci which should have been included in the source distribution */
#ifdef __GNUC__

#ifdef __linux__
/* glibc */
#include <features.h>
#else
/* NetBSD etc */
#include <sys/cdefs.h>
#ifdef __GNUC_PREREQ__
#define __GNUC_PREREQ __GNUC_PREREQ__
#endif
#endif

#ifndef __GNUC_PREREQ
/* fallback */
#define __GNUC_PREREQ(maj, min) 0
#endif

#if __GNUC_PREREQ(4,6)
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif

#endif

#include "loci_log.h"
#include "loci_int.h"

static void
ofdpa_l2_interface_remark_action_multipart_reply_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint8_t *)(buf + 0) = obj->version; /* version */
        *(uint8_t *)(buf + 1) = 0x13; /* type */
        *(uint16_t *)(buf + 8) = U16_HTON(0xffff); /* stats_type */
        *(uint32_t *)(buf + 16) = U32_HTON(0x1018); /* experimenter */
        *(uint32_t *)(buf + 20) = U32_HTON(0xb); /* subtype */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup ofdpa_l2_interface_remark_action_multipart_reply ofdpa_l2_interface_remark_action_multipart_reply
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
ofdpa_l2_interface_remark_action_multipart_reply_push_wire_values(ofdpa_l2_interface_remark_action_multipart_reply_t *obj)
{

    ofdpa_l2_interface_remark_action_multipart_reply_push_wire_types(obj);

    /* Message obj; set length */
    of_message_t msg;

    if ((msg = OF_OBJECT_TO_MESSAGE(obj)) != NULL) {
        of_message_length_set(msg, obj->length);
    }

    return OF_ERROR_NONE;
}

/**
 * Create a new ofdpa_l2_interface_remark_action_multipart_reply object
 *
 * @param version The wire version to use for the object
 * @return Pointer to the newly create object or NULL on error
 *
 * Initializes the new object with it's default fixed length associating
 * a new underlying wire buffer.
 *
 * Use new_from_message to bind an existing message to a message object,
 * or a _get function for non-message objects.
 *
 * \ingroup ofdpa_l2_interface_remark_action_multipart_reply
 */

ofdpa_l2_interface_remark_action_multipart_reply_t *
ofdpa_l2_interface_remark_action_multipart_reply_new(of_version_t version)
{
    ofdpa_l2_interface_remark_action_multipart_reply_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OFDPA_L2_INTERFACE_REMARK_ACTION_MULTIPART_REPLY] + of_object_extra_len[version][OFDPA_L2_INTERFACE_REMARK_ACTION_MULTIPART_REPLY];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (ofdpa_l2_interface_remark_action_multipart_reply_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    ofdpa_l2_interface_remark_action_multipart_reply_init(obj, version, bytes, 0);

    if (ofdpa_l2_interface_remark_action_multipart_reply_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type ofdpa_l2_interface_remark_action_multipart_reply.
 *
 * @param obj Pointer to the object to initialize
 * @param version The wire version to use for the object
 * @param bytes How many bytes in the object
 * @param clean_wire Boolean: If true, clear the wire object control struct
 *
 * If bytes < 0, then the default fixed length is used for the object
 *
 * This is a "coerce" function that sets up the pointers for the
 * accessors properly.
 *
 * If anything other than 0 is passed in for the buffer size, the underlying
 * wire buffer will have 'grow' called.
 */

void
ofdpa_l2_interface_remark_action_multipart_reply_init(ofdpa_l2_interface_remark_action_multipart_reply_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OFDPA_L2_INTERFACE_REMARK_ACTION_MULTIPART_REPLY] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OFDPA_L2_INTERFACE_REMARK_ACTION_MULTIPART_REPLY] + of_object_extra_len[version][OFDPA_L2_INTERFACE_REMARK_ACTION_MULTIPART_REPLY];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OFDPA_L2_INTERFACE_REMARK_ACTION_MULTIPART_REPLY;

    /* Set up the object's function pointers */

    obj->wire_type_set = ofdpa_l2_interface_remark_action_multipart_reply_push_wire_types;

    obj->wire_length_get = of_object_message_wire_length_get;
    obj->wire_length_set = of_object_message_wire_length_set;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Create a new ofdpa_l2_interface_remark_action_multipart_reply object and bind it to an existing message
 *
 * @param msg The message to bind the new object to
 * @return Pointer to the newly create object or NULL on error
 *
 * \ingroup ofdpa_l2_interface_remark_action_multipart_reply
 */

ofdpa_l2_interface_remark_action_multipart_reply_t *
ofdpa_l2_interface_remark_action_multipart_reply_new_from_message(of_message_t msg)
{
    ofdpa_l2_interface_remark_action_multipart_reply_t *obj = NULL;
    of_version_t version;
    int length;

    if (msg == NULL) return NULL;

    version = of_message_version_get(msg);
    if (!OF_VERSION_OKAY(version)) return NULL;

    length = of_message_length_get(msg);

    if ((obj = (ofdpa_l2_interface_remark_action_multipart_reply_t *)of_object_new(-1)) == NULL) {
        return NULL;
    }

    ofdpa_l2_interface_remark_action_multipart_reply_init(obj, version, 0, 0);

    if ((of_object_buffer_bind((of_object_t *)obj, OF_MESSAGE_TO_BUFFER(msg),
                               length, OF_MESSAGE_FREE_FUNCTION)) < 0) {
       FREE(obj);
       return NULL;
    }
    obj->length = length;
    obj->version = version;

    return obj;
}

/**
 * Get xid from an object of type ofdpa_l2_interface_remark_action_multipart_reply.
 * @param obj Pointer to an object of type ofdpa_l2_interface_remark_action_multipart_reply.
 * @param xid Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
ofdpa_l2_interface_remark_action_multipart_reply_xid_get(
    ofdpa_l2_interface_remark_action_multipart_reply_t *obj,
    uint32_t *xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_L2_INTERFACE_REMARK_ACTION_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, xid);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set xid in an object of type ofdpa_l2_interface_remark_action_multipart_reply.
 * @param obj Pointer to an object of type ofdpa_l2_interface_remark_action_multipart_reply.
 * @param xid The value to write into the object
 */
void
ofdpa_l2_interface_remark_action_multipart_reply_xid_set(
    ofdpa_l2_interface_remark_action_multipart_reply_t *obj,
    uint32_t xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_L2_INTERFACE_REMARK_ACTION_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, xid);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get flags from an object of type ofdpa_l2_interface_remark_action_multipart_reply.
 * @param obj Pointer to an object of type ofdpa_l2_interface_remark_action_multipart_reply.
 * @param flags Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
ofdpa_l2_interface_remark_action_multipart_reply_flags_get(
    ofdpa_l2_interface_remark_action_multipart_reply_t *obj,
    uint16_t *flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_L2_INTERFACE_REMARK_ACTION_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 10;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_get(wbuf, abs_offset, flags);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set flags in an object of type ofdpa_l2_interface_remark_action_multipart_reply.
 * @param obj Pointer to an object of type ofdpa_l2_interface_remark_action_multipart_reply.
 * @param flags The value to write into the object
 */
void
ofdpa_l2_interface_remark_action_multipart_reply_flags_set(
    ofdpa_l2_interface_remark_action_multipart_reply_t *obj,
    uint16_t flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_L2_INTERFACE_REMARK_ACTION_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 10;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_set(wbuf, abs_offset, flags);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get experimenter from an object of type ofdpa_l2_interface_remark_action_multipart_reply.
 * @param obj Pointer to an object of type ofdpa_l2_interface_remark_action_multipart_reply.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
ofdpa_l2_interface_remark_action_multipart_reply_experimenter_get(
    ofdpa_l2_interface_remark_action_multipart_reply_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_L2_INTERFACE_REMARK_ACTION_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 16;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set experimenter in an object of type ofdpa_l2_interface_remark_action_multipart_reply.
 * @param obj Pointer to an object of type ofdpa_l2_interface_remark_action_multipart_reply.
 * @param experimenter The value to write into the object
 */
void
ofdpa_l2_interface_remark_action_multipart_reply_experimenter_set(
    ofdpa_l2_interface_remark_action_multipart_reply_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_L2_INTERFACE_REMARK_ACTION_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 16;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get subtype from an object of type ofdpa_l2_interface_remark_action_multipart_reply.
 * @param obj Pointer to an object of type ofdpa_l2_interface_remark_action_multipart_reply.
 * @param subtype Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
ofdpa_l2_interface_remark_action_multipart_reply_subtype_get(
    ofdpa_l2_interface_remark_action_multipart_reply_t *obj,
    uint32_t *subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_L2_INTERFACE_REMARK_ACTION_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 20;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set subtype in an object of type ofdpa_l2_interface_remark_action_multipart_reply.
 * @param obj Pointer to an object of type ofdpa_l2_interface_remark_action_multipart_reply.
 * @param subtype The value to write into the object
 */
void
ofdpa_l2_interface_remark_action_multipart_reply_subtype_set(
    ofdpa_l2_interface_remark_action_multipart_reply_t *obj,
    uint32_t subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_L2_INTERFACE_REMARK_ACTION_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 20;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get index from an object of type ofdpa_l2_interface_remark_action_multipart_reply.
 * @param obj Pointer to an object of type ofdpa_l2_interface_remark_action_multipart_reply.
 * @param index Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
ofdpa_l2_interface_remark_action_multipart_reply_index_get(
    ofdpa_l2_interface_remark_action_multipart_reply_t *obj,
    uint32_t *index)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_L2_INTERFACE_REMARK_ACTION_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 24;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, index);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set index in an object of type ofdpa_l2_interface_remark_action_multipart_reply.
 * @param obj Pointer to an object of type ofdpa_l2_interface_remark_action_multipart_reply.
 * @param index The value to write into the object
 */
void
ofdpa_l2_interface_remark_action_multipart_reply_index_set(
    ofdpa_l2_interface_remark_action_multipart_reply_t *obj,
    uint32_t index)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_L2_INTERFACE_REMARK_ACTION_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 24;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, index);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get traffic_class from an object of type ofdpa_l2_interface_remark_action_multipart_reply.
 * @param obj Pointer to an object of type ofdpa_l2_interface_remark_action_multipart_reply.
 * @param traffic_class Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
ofdpa_l2_interface_remark_action_multipart_reply_traffic_class_get(
    ofdpa_l2_interface_remark_action_multipart_reply_t *obj,
    uint8_t *traffic_class)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_L2_INTERFACE_REMARK_ACTION_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 28;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_get(wbuf, abs_offset, traffic_class);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set traffic_class in an object of type ofdpa_l2_interface_remark_action_multipart_reply.
 * @param obj Pointer to an object of type ofdpa_l2_interface_remark_action_multipart_reply.
 * @param traffic_class The value to write into the object
 */
void
ofdpa_l2_interface_remark_action_multipart_reply_traffic_class_set(
    ofdpa_l2_interface_remark_action_multipart_reply_t *obj,
    uint8_t traffic_class)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_L2_INTERFACE_REMARK_ACTION_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 28;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_set(wbuf, abs_offset, traffic_class);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get color from an object of type ofdpa_l2_interface_remark_action_multipart_reply.
 * @param obj Pointer to an object of type ofdpa_l2_interface_remark_action_multipart_reply.
 * @param color Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
ofdpa_l2_interface_remark_action_multipart_reply_color_get(
    ofdpa_l2_interface_remark_action_multipart_reply_t *obj,
    uint8_t *color)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_L2_INTERFACE_REMARK_ACTION_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 29;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_get(wbuf, abs_offset, color);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set color in an object of type ofdpa_l2_interface_remark_action_multipart_reply.
 * @param obj Pointer to an object of type ofdpa_l2_interface_remark_action_multipart_reply.
 * @param color The value to write into the object
 */
void
ofdpa_l2_interface_remark_action_multipart_reply_color_set(
    ofdpa_l2_interface_remark_action_multipart_reply_t *obj,
    uint8_t color)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_L2_INTERFACE_REMARK_ACTION_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 29;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_set(wbuf, abs_offset, color);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get mpls_tc from an object of type ofdpa_l2_interface_remark_action_multipart_reply.
 * @param obj Pointer to an object of type ofdpa_l2_interface_remark_action_multipart_reply.
 * @param mpls_tc Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
ofdpa_l2_interface_remark_action_multipart_reply_mpls_tc_get(
    ofdpa_l2_interface_remark_action_multipart_reply_t *obj,
    uint8_t *mpls_tc)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_L2_INTERFACE_REMARK_ACTION_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 30;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_get(wbuf, abs_offset, mpls_tc);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set mpls_tc in an object of type ofdpa_l2_interface_remark_action_multipart_reply.
 * @param obj Pointer to an object of type ofdpa_l2_interface_remark_action_multipart_reply.
 * @param mpls_tc The value to write into the object
 */
void
ofdpa_l2_interface_remark_action_multipart_reply_mpls_tc_set(
    ofdpa_l2_interface_remark_action_multipart_reply_t *obj,
    uint8_t mpls_tc)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_L2_INTERFACE_REMARK_ACTION_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 30;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_set(wbuf, abs_offset, mpls_tc);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get vlan_pcp from an object of type ofdpa_l2_interface_remark_action_multipart_reply.
 * @param obj Pointer to an object of type ofdpa_l2_interface_remark_action_multipart_reply.
 * @param vlan_pcp Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
ofdpa_l2_interface_remark_action_multipart_reply_vlan_pcp_get(
    ofdpa_l2_interface_remark_action_multipart_reply_t *obj,
    uint8_t *vlan_pcp)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_L2_INTERFACE_REMARK_ACTION_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 31;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_get(wbuf, abs_offset, vlan_pcp);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set vlan_pcp in an object of type ofdpa_l2_interface_remark_action_multipart_reply.
 * @param obj Pointer to an object of type ofdpa_l2_interface_remark_action_multipart_reply.
 * @param vlan_pcp The value to write into the object
 */
void
ofdpa_l2_interface_remark_action_multipart_reply_vlan_pcp_set(
    ofdpa_l2_interface_remark_action_multipart_reply_t *obj,
    uint8_t vlan_pcp)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_L2_INTERFACE_REMARK_ACTION_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 31;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_set(wbuf, abs_offset, vlan_pcp);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get vlan_dei from an object of type ofdpa_l2_interface_remark_action_multipart_reply.
 * @param obj Pointer to an object of type ofdpa_l2_interface_remark_action_multipart_reply.
 * @param vlan_dei Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
ofdpa_l2_interface_remark_action_multipart_reply_vlan_dei_get(
    ofdpa_l2_interface_remark_action_multipart_reply_t *obj,
    uint8_t *vlan_dei)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_L2_INTERFACE_REMARK_ACTION_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 32;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_get(wbuf, abs_offset, vlan_dei);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set vlan_dei in an object of type ofdpa_l2_interface_remark_action_multipart_reply.
 * @param obj Pointer to an object of type ofdpa_l2_interface_remark_action_multipart_reply.
 * @param vlan_dei The value to write into the object
 */
void
ofdpa_l2_interface_remark_action_multipart_reply_vlan_dei_set(
    ofdpa_l2_interface_remark_action_multipart_reply_t *obj,
    uint8_t vlan_dei)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_L2_INTERFACE_REMARK_ACTION_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 32;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_set(wbuf, abs_offset, vlan_dei);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}
/* Copyright (c) 2008 The Board of Trustees of The Leland Stanford Junior University */
/* Copyright (c) 2011, 2012 Open Networking Foundation */
/* Copyright (c) 2012, 2013 Big Switch Networks, Inc. */
/* See the file LICENSE.loci which should have been included in the source distribution */
#ifdef __GNUC__

#ifdef __linux__
/* glibc */
#include <features.h>
#else
/* NetBSD etc */
#include <sys/cdefs.h>
#ifdef __GNUC_PREREQ__
#define __GNUC_PREREQ __GNUC_PREREQ__
#endif
#endif

#ifndef __GNUC_PREREQ
/* fallback */
#define __GNUC_PREREQ(maj, min) 0
#endif

#if __GNUC_PREREQ(4,6)
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif

#endif

#include "loci_log.h"
#include "loci_int.h"

static void
ofdpa_l2_interface_remark_action_multipart_request_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint8_t *)(buf + 0) = obj->version; /* version */
        *(uint8_t *)(buf + 1) = 0x12; /* type */
        *(uint16_t *)(buf + 8) = U16_HTON(0xffff); /* stats_type */
        *(uint32_t *)(buf + 16) = U32_HTON(0x1018); /* experimenter */
        *(uint32_t *)(buf + 20) = U32_HTON(0xb); /* subtype */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup ofdpa_l2_interface_remark_action_multipart_request ofdpa_l2_interface_remark_action_multipart_request
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
ofdpa_l2_interface_remark_action_multipart_request_push_wire_values(ofdpa_l2_interface_remark_action_multipart_request_t *obj)
{

    ofdpa_l2_interface_remark_action_multipart_request_push_wire_types(obj);

    /* Message obj; set length */
    of_message_t msg;

    if ((msg = OF_OBJECT_TO_MESSAGE(obj)) != NULL) {
        of_message_length_set(msg, obj->length);
    }

    return OF_ERROR_NONE;
}

/**
 * Create a new ofdpa_l2_interface_remark_action_multipart_request object
 *
 * @param version The wire version to use for the object
 * @return Pointer to the newly create object or NULL on error
 *
 * Initializes the new object with it's default fixed length associating
 * a new underlying wire buffer.
 *
 * Use new_from_message to bind an existing message to a message object,
 * or a _get function for non-message objects.
 *
 * \ingroup ofdpa_l2_interface_remark_action_multipart_request
 */

ofdpa_l2_interface_remark_action_multipart_request_t *
ofdpa_l2_interface_remark_action_multipart_request_new(of_version_t version)
{
    ofdpa_l2_interface_remark_action_multipart_request_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OFDPA_L2_INTERFACE_REMARK_ACTION_MULTIPART_REQUEST] + of_object_extra_len[version][OFDPA_L2_INTERFACE_REMARK_ACTION_MULTIPART_REQUEST];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (ofdpa_l2_interface_remark_action_multipart_request_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    ofdpa_l2_interface_remark_action_multipart_request_init(obj, version, bytes, 0);

    if (ofdpa_l2_interface_remark_action_multipart_request_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type ofdpa_l2_interface_remark_action_multipart_request.
 *
 * @param obj Pointer to the object to initialize
 * @param version The wire version to use for the object
 * @param bytes How many bytes in the object
 * @param clean_wire Boolean: If true, clear the wire object control struct
 *
 * If bytes < 0, then the default fixed length is used for the object
 *
 * This is a "coerce" function that sets up the pointers for the
 * accessors properly.
 *
 * If anything other than 0 is passed in for the buffer size, the underlying
 * wire buffer will have 'grow' called.
 */

void
ofdpa_l2_interface_remark_action_multipart_request_init(ofdpa_l2_interface_remark_action_multipart_request_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OFDPA_L2_INTERFACE_REMARK_ACTION_MULTIPART_REQUEST] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OFDPA_L2_INTERFACE_REMARK_ACTION_MULTIPART_REQUEST] + of_object_extra_len[version][OFDPA_L2_INTERFACE_REMARK_ACTION_MULTIPART_REQUEST];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OFDPA_L2_INTERFACE_REMARK_ACTION_MULTIPART_REQUEST;

    /* Set up the object's function pointers */

    obj->wire_type_set = ofdpa_l2_interface_remark_action_multipart_request_push_wire_types;

    obj->wire_length_get = of_object_message_wire_length_get;
    obj->wire_length_set = of_object_message_wire_length_set;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Create a new ofdpa_l2_interface_remark_action_multipart_request object and bind it to an existing message
 *
 * @param msg The message to bind the new object to
 * @return Pointer to the newly create object or NULL on error
 *
 * \ingroup ofdpa_l2_interface_remark_action_multipart_request
 */

ofdpa_l2_interface_remark_action_multipart_request_t *
ofdpa_l2_interface_remark_action_multipart_request_new_from_message(of_message_t msg)
{
    ofdpa_l2_interface_remark_action_multipart_request_t *obj = NULL;
    of_version_t version;
    int length;

    if (msg == NULL) return NULL;

    version = of_message_version_get(msg);
    if (!OF_VERSION_OKAY(version)) return NULL;

    length = of_message_length_get(msg);

    if ((obj = (ofdpa_l2_interface_remark_action_multipart_request_t *)of_object_new(-1)) == NULL) {
        return NULL;
    }

    ofdpa_l2_interface_remark_action_multipart_request_init(obj, version, 0, 0);

    if ((of_object_buffer_bind((of_object_t *)obj, OF_MESSAGE_TO_BUFFER(msg),
                               length, OF_MESSAGE_FREE_FUNCTION)) < 0) {
       FREE(obj);
       return NULL;
    }
    obj->length = length;
    obj->version = version;

    return obj;
}

/**
 * Get xid from an object of type ofdpa_l2_interface_remark_action_multipart_request.
 * @param obj Pointer to an object of type ofdpa_l2_interface_remark_action_multipart_request.
 * @param xid Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
ofdpa_l2_interface_remark_action_multipart_request_xid_get(
    ofdpa_l2_interface_remark_action_multipart_request_t *obj,
    uint32_t *xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_L2_INTERFACE_REMARK_ACTION_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, xid);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set xid in an object of type ofdpa_l2_interface_remark_action_multipart_request.
 * @param obj Pointer to an object of type ofdpa_l2_interface_remark_action_multipart_request.
 * @param xid The value to write into the object
 */
void
ofdpa_l2_interface_remark_action_multipart_request_xid_set(
    ofdpa_l2_interface_remark_action_multipart_request_t *obj,
    uint32_t xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_L2_INTERFACE_REMARK_ACTION_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, xid);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get flags from an object of type ofdpa_l2_interface_remark_action_multipart_request.
 * @param obj Pointer to an object of type ofdpa_l2_interface_remark_action_multipart_request.
 * @param flags Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
ofdpa_l2_interface_remark_action_multipart_request_flags_get(
    ofdpa_l2_interface_remark_action_multipart_request_t *obj,
    uint16_t *flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_L2_INTERFACE_REMARK_ACTION_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 10;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_get(wbuf, abs_offset, flags);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set flags in an object of type ofdpa_l2_interface_remark_action_multipart_request.
 * @param obj Pointer to an object of type ofdpa_l2_interface_remark_action_multipart_request.
 * @param flags The value to write into the object
 */
void
ofdpa_l2_interface_remark_action_multipart_request_flags_set(
    ofdpa_l2_interface_remark_action_multipart_request_t *obj,
    uint16_t flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_L2_INTERFACE_REMARK_ACTION_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 10;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_set(wbuf, abs_offset, flags);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get experimenter from an object of type ofdpa_l2_interface_remark_action_multipart_request.
 * @param obj Pointer to an object of type ofdpa_l2_interface_remark_action_multipart_request.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
ofdpa_l2_interface_remark_action_multipart_request_experimenter_get(
    ofdpa_l2_interface_remark_action_multipart_request_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_L2_INTERFACE_REMARK_ACTION_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 16;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set experimenter in an object of type ofdpa_l2_interface_remark_action_multipart_request.
 * @param obj Pointer to an object of type ofdpa_l2_interface_remark_action_multipart_request.
 * @param experimenter The value to write into the object
 */
void
ofdpa_l2_interface_remark_action_multipart_request_experimenter_set(
    ofdpa_l2_interface_remark_action_multipart_request_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_L2_INTERFACE_REMARK_ACTION_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 16;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get subtype from an object of type ofdpa_l2_interface_remark_action_multipart_request.
 * @param obj Pointer to an object of type ofdpa_l2_interface_remark_action_multipart_request.
 * @param subtype Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
ofdpa_l2_interface_remark_action_multipart_request_subtype_get(
    ofdpa_l2_interface_remark_action_multipart_request_t *obj,
    uint32_t *subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_L2_INTERFACE_REMARK_ACTION_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 20;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set subtype in an object of type ofdpa_l2_interface_remark_action_multipart_request.
 * @param obj Pointer to an object of type ofdpa_l2_interface_remark_action_multipart_request.
 * @param subtype The value to write into the object
 */
void
ofdpa_l2_interface_remark_action_multipart_request_subtype_set(
    ofdpa_l2_interface_remark_action_multipart_request_t *obj,
    uint32_t subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_L2_INTERFACE_REMARK_ACTION_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 20;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get index from an object of type ofdpa_l2_interface_remark_action_multipart_request.
 * @param obj Pointer to an object of type ofdpa_l2_interface_remark_action_multipart_request.
 * @param index Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
ofdpa_l2_interface_remark_action_multipart_request_index_get(
    ofdpa_l2_interface_remark_action_multipart_request_t *obj,
    uint32_t *index)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_L2_INTERFACE_REMARK_ACTION_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 24;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, index);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set index in an object of type ofdpa_l2_interface_remark_action_multipart_request.
 * @param obj Pointer to an object of type ofdpa_l2_interface_remark_action_multipart_request.
 * @param index The value to write into the object
 */
void
ofdpa_l2_interface_remark_action_multipart_request_index_set(
    ofdpa_l2_interface_remark_action_multipart_request_t *obj,
    uint32_t index)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_L2_INTERFACE_REMARK_ACTION_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 24;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, index);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get traffic_class from an object of type ofdpa_l2_interface_remark_action_multipart_request.
 * @param obj Pointer to an object of type ofdpa_l2_interface_remark_action_multipart_request.
 * @param traffic_class Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
ofdpa_l2_interface_remark_action_multipart_request_traffic_class_get(
    ofdpa_l2_interface_remark_action_multipart_request_t *obj,
    uint8_t *traffic_class)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_L2_INTERFACE_REMARK_ACTION_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 28;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_get(wbuf, abs_offset, traffic_class);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set traffic_class in an object of type ofdpa_l2_interface_remark_action_multipart_request.
 * @param obj Pointer to an object of type ofdpa_l2_interface_remark_action_multipart_request.
 * @param traffic_class The value to write into the object
 */
void
ofdpa_l2_interface_remark_action_multipart_request_traffic_class_set(
    ofdpa_l2_interface_remark_action_multipart_request_t *obj,
    uint8_t traffic_class)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_L2_INTERFACE_REMARK_ACTION_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 28;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_set(wbuf, abs_offset, traffic_class);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get color from an object of type ofdpa_l2_interface_remark_action_multipart_request.
 * @param obj Pointer to an object of type ofdpa_l2_interface_remark_action_multipart_request.
 * @param color Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
ofdpa_l2_interface_remark_action_multipart_request_color_get(
    ofdpa_l2_interface_remark_action_multipart_request_t *obj,
    uint8_t *color)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_L2_INTERFACE_REMARK_ACTION_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 29;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_get(wbuf, abs_offset, color);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set color in an object of type ofdpa_l2_interface_remark_action_multipart_request.
 * @param obj Pointer to an object of type ofdpa_l2_interface_remark_action_multipart_request.
 * @param color The value to write into the object
 */
void
ofdpa_l2_interface_remark_action_multipart_request_color_set(
    ofdpa_l2_interface_remark_action_multipart_request_t *obj,
    uint8_t color)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_L2_INTERFACE_REMARK_ACTION_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 29;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_set(wbuf, abs_offset, color);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get mpls_tc from an object of type ofdpa_l2_interface_remark_action_multipart_request.
 * @param obj Pointer to an object of type ofdpa_l2_interface_remark_action_multipart_request.
 * @param mpls_tc Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
ofdpa_l2_interface_remark_action_multipart_request_mpls_tc_get(
    ofdpa_l2_interface_remark_action_multipart_request_t *obj,
    uint8_t *mpls_tc)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_L2_INTERFACE_REMARK_ACTION_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 30;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_get(wbuf, abs_offset, mpls_tc);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set mpls_tc in an object of type ofdpa_l2_interface_remark_action_multipart_request.
 * @param obj Pointer to an object of type ofdpa_l2_interface_remark_action_multipart_request.
 * @param mpls_tc The value to write into the object
 */
void
ofdpa_l2_interface_remark_action_multipart_request_mpls_tc_set(
    ofdpa_l2_interface_remark_action_multipart_request_t *obj,
    uint8_t mpls_tc)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_L2_INTERFACE_REMARK_ACTION_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 30;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_set(wbuf, abs_offset, mpls_tc);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get vlan_pcp from an object of type ofdpa_l2_interface_remark_action_multipart_request.
 * @param obj Pointer to an object of type ofdpa_l2_interface_remark_action_multipart_request.
 * @param vlan_pcp Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
ofdpa_l2_interface_remark_action_multipart_request_vlan_pcp_get(
    ofdpa_l2_interface_remark_action_multipart_request_t *obj,
    uint8_t *vlan_pcp)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_L2_INTERFACE_REMARK_ACTION_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 31;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_get(wbuf, abs_offset, vlan_pcp);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set vlan_pcp in an object of type ofdpa_l2_interface_remark_action_multipart_request.
 * @param obj Pointer to an object of type ofdpa_l2_interface_remark_action_multipart_request.
 * @param vlan_pcp The value to write into the object
 */
void
ofdpa_l2_interface_remark_action_multipart_request_vlan_pcp_set(
    ofdpa_l2_interface_remark_action_multipart_request_t *obj,
    uint8_t vlan_pcp)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_L2_INTERFACE_REMARK_ACTION_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 31;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_set(wbuf, abs_offset, vlan_pcp);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get vlan_dei from an object of type ofdpa_l2_interface_remark_action_multipart_request.
 * @param obj Pointer to an object of type ofdpa_l2_interface_remark_action_multipart_request.
 * @param vlan_dei Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
ofdpa_l2_interface_remark_action_multipart_request_vlan_dei_get(
    ofdpa_l2_interface_remark_action_multipart_request_t *obj,
    uint8_t *vlan_dei)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_L2_INTERFACE_REMARK_ACTION_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 32;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_get(wbuf, abs_offset, vlan_dei);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set vlan_dei in an object of type ofdpa_l2_interface_remark_action_multipart_request.
 * @param obj Pointer to an object of type ofdpa_l2_interface_remark_action_multipart_request.
 * @param vlan_dei The value to write into the object
 */
void
ofdpa_l2_interface_remark_action_multipart_request_vlan_dei_set(
    ofdpa_l2_interface_remark_action_multipart_request_t *obj,
    uint8_t vlan_dei)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_L2_INTERFACE_REMARK_ACTION_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 32;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_set(wbuf, abs_offset, vlan_dei);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}
/* Copyright (c) 2008 The Board of Trustees of The Leland Stanford Junior University */
/* Copyright (c) 2011, 2012 Open Networking Foundation */
/* Copyright (c) 2012, 2013 Big Switch Networks, Inc. */
/* See the file LICENSE.loci which should have been included in the source distribution */
#ifdef __GNUC__

#ifdef __linux__
/* glibc */
#include <features.h>
#else
/* NetBSD etc */
#include <sys/cdefs.h>
#ifdef __GNUC_PREREQ__
#define __GNUC_PREREQ __GNUC_PREREQ__
#endif
#endif

#ifndef __GNUC_PREREQ
/* fallback */
#define __GNUC_PREREQ(maj, min) 0
#endif

#if __GNUC_PREREQ(4,6)
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif

#endif

#include "loci_log.h"
#include "loci_int.h"

static void
ofdpa_mpls_set_qos_action_mod_msg_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint8_t *)(buf + 0) = obj->version; /* version */
        *(uint8_t *)(buf + 1) = 0x4; /* type */
        *(uint32_t *)(buf + 8) = U32_HTON(0x1018); /* experimenter */
        *(uint32_t *)(buf + 12) = U32_HTON(0x0); /* subtype */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup ofdpa_mpls_set_qos_action_mod_msg ofdpa_mpls_set_qos_action_mod_msg
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
ofdpa_mpls_set_qos_action_mod_msg_push_wire_values(ofdpa_mpls_set_qos_action_mod_msg_t *obj)
{

    ofdpa_mpls_set_qos_action_mod_msg_push_wire_types(obj);

    /* Message obj; set length */
    of_message_t msg;

    if ((msg = OF_OBJECT_TO_MESSAGE(obj)) != NULL) {
        of_message_length_set(msg, obj->length);
    }

    return OF_ERROR_NONE;
}

/**
 * Create a new ofdpa_mpls_set_qos_action_mod_msg object
 *
 * @param version The wire version to use for the object
 * @return Pointer to the newly create object or NULL on error
 *
 * Initializes the new object with it's default fixed length associating
 * a new underlying wire buffer.
 *
 * Use new_from_message to bind an existing message to a message object,
 * or a _get function for non-message objects.
 *
 * \ingroup ofdpa_mpls_set_qos_action_mod_msg
 */

ofdpa_mpls_set_qos_action_mod_msg_t *
ofdpa_mpls_set_qos_action_mod_msg_new(of_version_t version)
{
    ofdpa_mpls_set_qos_action_mod_msg_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OFDPA_MPLS_SET_QOS_ACTION_MOD_MSG] + of_object_extra_len[version][OFDPA_MPLS_SET_QOS_ACTION_MOD_MSG];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (ofdpa_mpls_set_qos_action_mod_msg_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    ofdpa_mpls_set_qos_action_mod_msg_init(obj, version, bytes, 0);

    if (ofdpa_mpls_set_qos_action_mod_msg_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type ofdpa_mpls_set_qos_action_mod_msg.
 *
 * @param obj Pointer to the object to initialize
 * @param version The wire version to use for the object
 * @param bytes How many bytes in the object
 * @param clean_wire Boolean: If true, clear the wire object control struct
 *
 * If bytes < 0, then the default fixed length is used for the object
 *
 * This is a "coerce" function that sets up the pointers for the
 * accessors properly.
 *
 * If anything other than 0 is passed in for the buffer size, the underlying
 * wire buffer will have 'grow' called.
 */

void
ofdpa_mpls_set_qos_action_mod_msg_init(ofdpa_mpls_set_qos_action_mod_msg_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OFDPA_MPLS_SET_QOS_ACTION_MOD_MSG] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OFDPA_MPLS_SET_QOS_ACTION_MOD_MSG] + of_object_extra_len[version][OFDPA_MPLS_SET_QOS_ACTION_MOD_MSG];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OFDPA_MPLS_SET_QOS_ACTION_MOD_MSG;

    /* Set up the object's function pointers */

    obj->wire_type_set = ofdpa_mpls_set_qos_action_mod_msg_push_wire_types;

    obj->wire_length_get = of_object_message_wire_length_get;
    obj->wire_length_set = of_object_message_wire_length_set;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Create a new ofdpa_mpls_set_qos_action_mod_msg object and bind it to an existing message
 *
 * @param msg The message to bind the new object to
 * @return Pointer to the newly create object or NULL on error
 *
 * \ingroup ofdpa_mpls_set_qos_action_mod_msg
 */

ofdpa_mpls_set_qos_action_mod_msg_t *
ofdpa_mpls_set_qos_action_mod_msg_new_from_message(of_message_t msg)
{
    ofdpa_mpls_set_qos_action_mod_msg_t *obj = NULL;
    of_version_t version;
    int length;

    if (msg == NULL) return NULL;

    version = of_message_version_get(msg);
    if (!OF_VERSION_OKAY(version)) return NULL;

    length = of_message_length_get(msg);

    if ((obj = (ofdpa_mpls_set_qos_action_mod_msg_t *)of_object_new(-1)) == NULL) {
        return NULL;
    }

    ofdpa_mpls_set_qos_action_mod_msg_init(obj, version, 0, 0);

    if ((of_object_buffer_bind((of_object_t *)obj, OF_MESSAGE_TO_BUFFER(msg),
                               length, OF_MESSAGE_FREE_FUNCTION)) < 0) {
       FREE(obj);
       return NULL;
    }
    obj->length = length;
    obj->version = version;

    return obj;
}

/**
 * Get xid from an object of type ofdpa_mpls_set_qos_action_mod_msg.
 * @param obj Pointer to an object of type ofdpa_mpls_set_qos_action_mod_msg.
 * @param xid Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
ofdpa_mpls_set_qos_action_mod_msg_xid_get(
    ofdpa_mpls_set_qos_action_mod_msg_t *obj,
    uint32_t *xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_SET_QOS_ACTION_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, xid);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set xid in an object of type ofdpa_mpls_set_qos_action_mod_msg.
 * @param obj Pointer to an object of type ofdpa_mpls_set_qos_action_mod_msg.
 * @param xid The value to write into the object
 */
void
ofdpa_mpls_set_qos_action_mod_msg_xid_set(
    ofdpa_mpls_set_qos_action_mod_msg_t *obj,
    uint32_t xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_SET_QOS_ACTION_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, xid);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get experimenter from an object of type ofdpa_mpls_set_qos_action_mod_msg.
 * @param obj Pointer to an object of type ofdpa_mpls_set_qos_action_mod_msg.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
ofdpa_mpls_set_qos_action_mod_msg_experimenter_get(
    ofdpa_mpls_set_qos_action_mod_msg_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_SET_QOS_ACTION_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 8;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set experimenter in an object of type ofdpa_mpls_set_qos_action_mod_msg.
 * @param obj Pointer to an object of type ofdpa_mpls_set_qos_action_mod_msg.
 * @param experimenter The value to write into the object
 */
void
ofdpa_mpls_set_qos_action_mod_msg_experimenter_set(
    ofdpa_mpls_set_qos_action_mod_msg_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_SET_QOS_ACTION_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 8;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get subtype from an object of type ofdpa_mpls_set_qos_action_mod_msg.
 * @param obj Pointer to an object of type ofdpa_mpls_set_qos_action_mod_msg.
 * @param subtype Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
ofdpa_mpls_set_qos_action_mod_msg_subtype_get(
    ofdpa_mpls_set_qos_action_mod_msg_t *obj,
    uint32_t *subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_SET_QOS_ACTION_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 12;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set subtype in an object of type ofdpa_mpls_set_qos_action_mod_msg.
 * @param obj Pointer to an object of type ofdpa_mpls_set_qos_action_mod_msg.
 * @param subtype The value to write into the object
 */
void
ofdpa_mpls_set_qos_action_mod_msg_subtype_set(
    ofdpa_mpls_set_qos_action_mod_msg_t *obj,
    uint32_t subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_SET_QOS_ACTION_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 12;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get mod_command from an object of type ofdpa_mpls_set_qos_action_mod_msg.
 * @param obj Pointer to an object of type ofdpa_mpls_set_qos_action_mod_msg.
 * @param mod_command Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
ofdpa_mpls_set_qos_action_mod_msg_mod_command_get(
    ofdpa_mpls_set_qos_action_mod_msg_t *obj,
    uint32_t *mod_command)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_SET_QOS_ACTION_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 16;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, mod_command);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set mod_command in an object of type ofdpa_mpls_set_qos_action_mod_msg.
 * @param obj Pointer to an object of type ofdpa_mpls_set_qos_action_mod_msg.
 * @param mod_command The value to write into the object
 */
void
ofdpa_mpls_set_qos_action_mod_msg_mod_command_set(
    ofdpa_mpls_set_qos_action_mod_msg_t *obj,
    uint32_t mod_command)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_SET_QOS_ACTION_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 16;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, mod_command);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get qos_index from an object of type ofdpa_mpls_set_qos_action_mod_msg.
 * @param obj Pointer to an object of type ofdpa_mpls_set_qos_action_mod_msg.
 * @param qos_index Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
ofdpa_mpls_set_qos_action_mod_msg_qos_index_get(
    ofdpa_mpls_set_qos_action_mod_msg_t *obj,
    uint8_t *qos_index)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_SET_QOS_ACTION_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 20;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_get(wbuf, abs_offset, qos_index);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set qos_index in an object of type ofdpa_mpls_set_qos_action_mod_msg.
 * @param obj Pointer to an object of type ofdpa_mpls_set_qos_action_mod_msg.
 * @param qos_index The value to write into the object
 */
void
ofdpa_mpls_set_qos_action_mod_msg_qos_index_set(
    ofdpa_mpls_set_qos_action_mod_msg_t *obj,
    uint8_t qos_index)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_SET_QOS_ACTION_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 20;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_set(wbuf, abs_offset, qos_index);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get mpls_tc from an object of type ofdpa_mpls_set_qos_action_mod_msg.
 * @param obj Pointer to an object of type ofdpa_mpls_set_qos_action_mod_msg.
 * @param mpls_tc Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
ofdpa_mpls_set_qos_action_mod_msg_mpls_tc_get(
    ofdpa_mpls_set_qos_action_mod_msg_t *obj,
    uint8_t *mpls_tc)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_SET_QOS_ACTION_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 21;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_get(wbuf, abs_offset, mpls_tc);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set mpls_tc in an object of type ofdpa_mpls_set_qos_action_mod_msg.
 * @param obj Pointer to an object of type ofdpa_mpls_set_qos_action_mod_msg.
 * @param mpls_tc The value to write into the object
 */
void
ofdpa_mpls_set_qos_action_mod_msg_mpls_tc_set(
    ofdpa_mpls_set_qos_action_mod_msg_t *obj,
    uint8_t mpls_tc)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_SET_QOS_ACTION_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 21;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_set(wbuf, abs_offset, mpls_tc);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get traffic_class from an object of type ofdpa_mpls_set_qos_action_mod_msg.
 * @param obj Pointer to an object of type ofdpa_mpls_set_qos_action_mod_msg.
 * @param traffic_class Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
ofdpa_mpls_set_qos_action_mod_msg_traffic_class_get(
    ofdpa_mpls_set_qos_action_mod_msg_t *obj,
    uint8_t *traffic_class)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_SET_QOS_ACTION_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 22;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_get(wbuf, abs_offset, traffic_class);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set traffic_class in an object of type ofdpa_mpls_set_qos_action_mod_msg.
 * @param obj Pointer to an object of type ofdpa_mpls_set_qos_action_mod_msg.
 * @param traffic_class The value to write into the object
 */
void
ofdpa_mpls_set_qos_action_mod_msg_traffic_class_set(
    ofdpa_mpls_set_qos_action_mod_msg_t *obj,
    uint8_t traffic_class)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_SET_QOS_ACTION_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 22;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_set(wbuf, abs_offset, traffic_class);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get color from an object of type ofdpa_mpls_set_qos_action_mod_msg.
 * @param obj Pointer to an object of type ofdpa_mpls_set_qos_action_mod_msg.
 * @param color Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
ofdpa_mpls_set_qos_action_mod_msg_color_get(
    ofdpa_mpls_set_qos_action_mod_msg_t *obj,
    uint8_t *color)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_SET_QOS_ACTION_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 23;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_get(wbuf, abs_offset, color);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set color in an object of type ofdpa_mpls_set_qos_action_mod_msg.
 * @param obj Pointer to an object of type ofdpa_mpls_set_qos_action_mod_msg.
 * @param color The value to write into the object
 */
void
ofdpa_mpls_set_qos_action_mod_msg_color_set(
    ofdpa_mpls_set_qos_action_mod_msg_t *obj,
    uint8_t color)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_SET_QOS_ACTION_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 23;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_set(wbuf, abs_offset, color);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}
/* Copyright (c) 2008 The Board of Trustees of The Leland Stanford Junior University */
/* Copyright (c) 2011, 2012 Open Networking Foundation */
/* Copyright (c) 2012, 2013 Big Switch Networks, Inc. */
/* See the file LICENSE.loci which should have been included in the source distribution */
#ifdef __GNUC__

#ifdef __linux__
/* glibc */
#include <features.h>
#else
/* NetBSD etc */
#include <sys/cdefs.h>
#ifdef __GNUC_PREREQ__
#define __GNUC_PREREQ __GNUC_PREREQ__
#endif
#endif

#ifndef __GNUC_PREREQ
/* fallback */
#define __GNUC_PREREQ(maj, min) 0
#endif

#if __GNUC_PREREQ(4,6)
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif

#endif

#include "loci_log.h"
#include "loci_int.h"

static void
ofdpa_mpls_set_qos_action_multipart_reply_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint8_t *)(buf + 0) = obj->version; /* version */
        *(uint8_t *)(buf + 1) = 0x13; /* type */
        *(uint16_t *)(buf + 8) = U16_HTON(0xffff); /* stats_type */
        *(uint32_t *)(buf + 16) = U32_HTON(0x1018); /* experimenter */
        *(uint32_t *)(buf + 20) = U32_HTON(0x1); /* subtype */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup ofdpa_mpls_set_qos_action_multipart_reply ofdpa_mpls_set_qos_action_multipart_reply
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
ofdpa_mpls_set_qos_action_multipart_reply_push_wire_values(ofdpa_mpls_set_qos_action_multipart_reply_t *obj)
{

    ofdpa_mpls_set_qos_action_multipart_reply_push_wire_types(obj);

    /* Message obj; set length */
    of_message_t msg;

    if ((msg = OF_OBJECT_TO_MESSAGE(obj)) != NULL) {
        of_message_length_set(msg, obj->length);
    }

    return OF_ERROR_NONE;
}

/**
 * Create a new ofdpa_mpls_set_qos_action_multipart_reply object
 *
 * @param version The wire version to use for the object
 * @return Pointer to the newly create object or NULL on error
 *
 * Initializes the new object with it's default fixed length associating
 * a new underlying wire buffer.
 *
 * Use new_from_message to bind an existing message to a message object,
 * or a _get function for non-message objects.
 *
 * \ingroup ofdpa_mpls_set_qos_action_multipart_reply
 */

ofdpa_mpls_set_qos_action_multipart_reply_t *
ofdpa_mpls_set_qos_action_multipart_reply_new(of_version_t version)
{
    ofdpa_mpls_set_qos_action_multipart_reply_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OFDPA_MPLS_SET_QOS_ACTION_MULTIPART_REPLY] + of_object_extra_len[version][OFDPA_MPLS_SET_QOS_ACTION_MULTIPART_REPLY];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (ofdpa_mpls_set_qos_action_multipart_reply_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    ofdpa_mpls_set_qos_action_multipart_reply_init(obj, version, bytes, 0);

    if (ofdpa_mpls_set_qos_action_multipart_reply_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type ofdpa_mpls_set_qos_action_multipart_reply.
 *
 * @param obj Pointer to the object to initialize
 * @param version The wire version to use for the object
 * @param bytes How many bytes in the object
 * @param clean_wire Boolean: If true, clear the wire object control struct
 *
 * If bytes < 0, then the default fixed length is used for the object
 *
 * This is a "coerce" function that sets up the pointers for the
 * accessors properly.
 *
 * If anything other than 0 is passed in for the buffer size, the underlying
 * wire buffer will have 'grow' called.
 */

void
ofdpa_mpls_set_qos_action_multipart_reply_init(ofdpa_mpls_set_qos_action_multipart_reply_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OFDPA_MPLS_SET_QOS_ACTION_MULTIPART_REPLY] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OFDPA_MPLS_SET_QOS_ACTION_MULTIPART_REPLY] + of_object_extra_len[version][OFDPA_MPLS_SET_QOS_ACTION_MULTIPART_REPLY];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OFDPA_MPLS_SET_QOS_ACTION_MULTIPART_REPLY;

    /* Set up the object's function pointers */

    obj->wire_type_set = ofdpa_mpls_set_qos_action_multipart_reply_push_wire_types;

    obj->wire_length_get = of_object_message_wire_length_get;
    obj->wire_length_set = of_object_message_wire_length_set;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Create a new ofdpa_mpls_set_qos_action_multipart_reply object and bind it to an existing message
 *
 * @param msg The message to bind the new object to
 * @return Pointer to the newly create object or NULL on error
 *
 * \ingroup ofdpa_mpls_set_qos_action_multipart_reply
 */

ofdpa_mpls_set_qos_action_multipart_reply_t *
ofdpa_mpls_set_qos_action_multipart_reply_new_from_message(of_message_t msg)
{
    ofdpa_mpls_set_qos_action_multipart_reply_t *obj = NULL;
    of_version_t version;
    int length;

    if (msg == NULL) return NULL;

    version = of_message_version_get(msg);
    if (!OF_VERSION_OKAY(version)) return NULL;

    length = of_message_length_get(msg);

    if ((obj = (ofdpa_mpls_set_qos_action_multipart_reply_t *)of_object_new(-1)) == NULL) {
        return NULL;
    }

    ofdpa_mpls_set_qos_action_multipart_reply_init(obj, version, 0, 0);

    if ((of_object_buffer_bind((of_object_t *)obj, OF_MESSAGE_TO_BUFFER(msg),
                               length, OF_MESSAGE_FREE_FUNCTION)) < 0) {
       FREE(obj);
       return NULL;
    }
    obj->length = length;
    obj->version = version;

    return obj;
}

/**
 * Get xid from an object of type ofdpa_mpls_set_qos_action_multipart_reply.
 * @param obj Pointer to an object of type ofdpa_mpls_set_qos_action_multipart_reply.
 * @param xid Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
ofdpa_mpls_set_qos_action_multipart_reply_xid_get(
    ofdpa_mpls_set_qos_action_multipart_reply_t *obj,
    uint32_t *xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_SET_QOS_ACTION_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, xid);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set xid in an object of type ofdpa_mpls_set_qos_action_multipart_reply.
 * @param obj Pointer to an object of type ofdpa_mpls_set_qos_action_multipart_reply.
 * @param xid The value to write into the object
 */
void
ofdpa_mpls_set_qos_action_multipart_reply_xid_set(
    ofdpa_mpls_set_qos_action_multipart_reply_t *obj,
    uint32_t xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_SET_QOS_ACTION_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, xid);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get flags from an object of type ofdpa_mpls_set_qos_action_multipart_reply.
 * @param obj Pointer to an object of type ofdpa_mpls_set_qos_action_multipart_reply.
 * @param flags Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
ofdpa_mpls_set_qos_action_multipart_reply_flags_get(
    ofdpa_mpls_set_qos_action_multipart_reply_t *obj,
    uint16_t *flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_SET_QOS_ACTION_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 10;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_get(wbuf, abs_offset, flags);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set flags in an object of type ofdpa_mpls_set_qos_action_multipart_reply.
 * @param obj Pointer to an object of type ofdpa_mpls_set_qos_action_multipart_reply.
 * @param flags The value to write into the object
 */
void
ofdpa_mpls_set_qos_action_multipart_reply_flags_set(
    ofdpa_mpls_set_qos_action_multipart_reply_t *obj,
    uint16_t flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_SET_QOS_ACTION_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 10;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_set(wbuf, abs_offset, flags);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get experimenter from an object of type ofdpa_mpls_set_qos_action_multipart_reply.
 * @param obj Pointer to an object of type ofdpa_mpls_set_qos_action_multipart_reply.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
ofdpa_mpls_set_qos_action_multipart_reply_experimenter_get(
    ofdpa_mpls_set_qos_action_multipart_reply_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_SET_QOS_ACTION_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 16;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set experimenter in an object of type ofdpa_mpls_set_qos_action_multipart_reply.
 * @param obj Pointer to an object of type ofdpa_mpls_set_qos_action_multipart_reply.
 * @param experimenter The value to write into the object
 */
void
ofdpa_mpls_set_qos_action_multipart_reply_experimenter_set(
    ofdpa_mpls_set_qos_action_multipart_reply_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_SET_QOS_ACTION_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 16;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get subtype from an object of type ofdpa_mpls_set_qos_action_multipart_reply.
 * @param obj Pointer to an object of type ofdpa_mpls_set_qos_action_multipart_reply.
 * @param subtype Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
ofdpa_mpls_set_qos_action_multipart_reply_subtype_get(
    ofdpa_mpls_set_qos_action_multipart_reply_t *obj,
    uint32_t *subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_SET_QOS_ACTION_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 20;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set subtype in an object of type ofdpa_mpls_set_qos_action_multipart_reply.
 * @param obj Pointer to an object of type ofdpa_mpls_set_qos_action_multipart_reply.
 * @param subtype The value to write into the object
 */
void
ofdpa_mpls_set_qos_action_multipart_reply_subtype_set(
    ofdpa_mpls_set_qos_action_multipart_reply_t *obj,
    uint32_t subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_SET_QOS_ACTION_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 20;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get qos_index from an object of type ofdpa_mpls_set_qos_action_multipart_reply.
 * @param obj Pointer to an object of type ofdpa_mpls_set_qos_action_multipart_reply.
 * @param qos_index Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
ofdpa_mpls_set_qos_action_multipart_reply_qos_index_get(
    ofdpa_mpls_set_qos_action_multipart_reply_t *obj,
    uint8_t *qos_index)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_SET_QOS_ACTION_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 24;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_get(wbuf, abs_offset, qos_index);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set qos_index in an object of type ofdpa_mpls_set_qos_action_multipart_reply.
 * @param obj Pointer to an object of type ofdpa_mpls_set_qos_action_multipart_reply.
 * @param qos_index The value to write into the object
 */
void
ofdpa_mpls_set_qos_action_multipart_reply_qos_index_set(
    ofdpa_mpls_set_qos_action_multipart_reply_t *obj,
    uint8_t qos_index)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_SET_QOS_ACTION_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 24;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_set(wbuf, abs_offset, qos_index);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get mpls_tc from an object of type ofdpa_mpls_set_qos_action_multipart_reply.
 * @param obj Pointer to an object of type ofdpa_mpls_set_qos_action_multipart_reply.
 * @param mpls_tc Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
ofdpa_mpls_set_qos_action_multipart_reply_mpls_tc_get(
    ofdpa_mpls_set_qos_action_multipart_reply_t *obj,
    uint8_t *mpls_tc)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_SET_QOS_ACTION_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 25;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_get(wbuf, abs_offset, mpls_tc);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set mpls_tc in an object of type ofdpa_mpls_set_qos_action_multipart_reply.
 * @param obj Pointer to an object of type ofdpa_mpls_set_qos_action_multipart_reply.
 * @param mpls_tc The value to write into the object
 */
void
ofdpa_mpls_set_qos_action_multipart_reply_mpls_tc_set(
    ofdpa_mpls_set_qos_action_multipart_reply_t *obj,
    uint8_t mpls_tc)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_SET_QOS_ACTION_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 25;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_set(wbuf, abs_offset, mpls_tc);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get traffic_class from an object of type ofdpa_mpls_set_qos_action_multipart_reply.
 * @param obj Pointer to an object of type ofdpa_mpls_set_qos_action_multipart_reply.
 * @param traffic_class Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
ofdpa_mpls_set_qos_action_multipart_reply_traffic_class_get(
    ofdpa_mpls_set_qos_action_multipart_reply_t *obj,
    uint8_t *traffic_class)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_SET_QOS_ACTION_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 26;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_get(wbuf, abs_offset, traffic_class);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set traffic_class in an object of type ofdpa_mpls_set_qos_action_multipart_reply.
 * @param obj Pointer to an object of type ofdpa_mpls_set_qos_action_multipart_reply.
 * @param traffic_class The value to write into the object
 */
void
ofdpa_mpls_set_qos_action_multipart_reply_traffic_class_set(
    ofdpa_mpls_set_qos_action_multipart_reply_t *obj,
    uint8_t traffic_class)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_SET_QOS_ACTION_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 26;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_set(wbuf, abs_offset, traffic_class);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get color from an object of type ofdpa_mpls_set_qos_action_multipart_reply.
 * @param obj Pointer to an object of type ofdpa_mpls_set_qos_action_multipart_reply.
 * @param color Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
ofdpa_mpls_set_qos_action_multipart_reply_color_get(
    ofdpa_mpls_set_qos_action_multipart_reply_t *obj,
    uint8_t *color)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_SET_QOS_ACTION_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 27;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_get(wbuf, abs_offset, color);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set color in an object of type ofdpa_mpls_set_qos_action_multipart_reply.
 * @param obj Pointer to an object of type ofdpa_mpls_set_qos_action_multipart_reply.
 * @param color The value to write into the object
 */
void
ofdpa_mpls_set_qos_action_multipart_reply_color_set(
    ofdpa_mpls_set_qos_action_multipart_reply_t *obj,
    uint8_t color)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_SET_QOS_ACTION_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 27;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_set(wbuf, abs_offset, color);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}
/* Copyright (c) 2008 The Board of Trustees of The Leland Stanford Junior University */
/* Copyright (c) 2011, 2012 Open Networking Foundation */
/* Copyright (c) 2012, 2013 Big Switch Networks, Inc. */
/* See the file LICENSE.loci which should have been included in the source distribution */
#ifdef __GNUC__

#ifdef __linux__
/* glibc */
#include <features.h>
#else
/* NetBSD etc */
#include <sys/cdefs.h>
#ifdef __GNUC_PREREQ__
#define __GNUC_PREREQ __GNUC_PREREQ__
#endif
#endif

#ifndef __GNUC_PREREQ
/* fallback */
#define __GNUC_PREREQ(maj, min) 0
#endif

#if __GNUC_PREREQ(4,6)
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif

#endif

#include "loci_log.h"
#include "loci_int.h"

static void
ofdpa_mpls_set_qos_action_multipart_request_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint8_t *)(buf + 0) = obj->version; /* version */
        *(uint8_t *)(buf + 1) = 0x12; /* type */
        *(uint16_t *)(buf + 8) = U16_HTON(0xffff); /* stats_type */
        *(uint32_t *)(buf + 16) = U32_HTON(0x1018); /* experimenter */
        *(uint32_t *)(buf + 20) = U32_HTON(0x1); /* subtype */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup ofdpa_mpls_set_qos_action_multipart_request ofdpa_mpls_set_qos_action_multipart_request
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
ofdpa_mpls_set_qos_action_multipart_request_push_wire_values(ofdpa_mpls_set_qos_action_multipart_request_t *obj)
{

    ofdpa_mpls_set_qos_action_multipart_request_push_wire_types(obj);

    /* Message obj; set length */
    of_message_t msg;

    if ((msg = OF_OBJECT_TO_MESSAGE(obj)) != NULL) {
        of_message_length_set(msg, obj->length);
    }

    return OF_ERROR_NONE;
}

/**
 * Create a new ofdpa_mpls_set_qos_action_multipart_request object
 *
 * @param version The wire version to use for the object
 * @return Pointer to the newly create object or NULL on error
 *
 * Initializes the new object with it's default fixed length associating
 * a new underlying wire buffer.
 *
 * Use new_from_message to bind an existing message to a message object,
 * or a _get function for non-message objects.
 *
 * \ingroup ofdpa_mpls_set_qos_action_multipart_request
 */

ofdpa_mpls_set_qos_action_multipart_request_t *
ofdpa_mpls_set_qos_action_multipart_request_new(of_version_t version)
{
    ofdpa_mpls_set_qos_action_multipart_request_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OFDPA_MPLS_SET_QOS_ACTION_MULTIPART_REQUEST] + of_object_extra_len[version][OFDPA_MPLS_SET_QOS_ACTION_MULTIPART_REQUEST];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (ofdpa_mpls_set_qos_action_multipart_request_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    ofdpa_mpls_set_qos_action_multipart_request_init(obj, version, bytes, 0);

    if (ofdpa_mpls_set_qos_action_multipart_request_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type ofdpa_mpls_set_qos_action_multipart_request.
 *
 * @param obj Pointer to the object to initialize
 * @param version The wire version to use for the object
 * @param bytes How many bytes in the object
 * @param clean_wire Boolean: If true, clear the wire object control struct
 *
 * If bytes < 0, then the default fixed length is used for the object
 *
 * This is a "coerce" function that sets up the pointers for the
 * accessors properly.
 *
 * If anything other than 0 is passed in for the buffer size, the underlying
 * wire buffer will have 'grow' called.
 */

void
ofdpa_mpls_set_qos_action_multipart_request_init(ofdpa_mpls_set_qos_action_multipart_request_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OFDPA_MPLS_SET_QOS_ACTION_MULTIPART_REQUEST] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OFDPA_MPLS_SET_QOS_ACTION_MULTIPART_REQUEST] + of_object_extra_len[version][OFDPA_MPLS_SET_QOS_ACTION_MULTIPART_REQUEST];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OFDPA_MPLS_SET_QOS_ACTION_MULTIPART_REQUEST;

    /* Set up the object's function pointers */

    obj->wire_type_set = ofdpa_mpls_set_qos_action_multipart_request_push_wire_types;

    obj->wire_length_get = of_object_message_wire_length_get;
    obj->wire_length_set = of_object_message_wire_length_set;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Create a new ofdpa_mpls_set_qos_action_multipart_request object and bind it to an existing message
 *
 * @param msg The message to bind the new object to
 * @return Pointer to the newly create object or NULL on error
 *
 * \ingroup ofdpa_mpls_set_qos_action_multipart_request
 */

ofdpa_mpls_set_qos_action_multipart_request_t *
ofdpa_mpls_set_qos_action_multipart_request_new_from_message(of_message_t msg)
{
    ofdpa_mpls_set_qos_action_multipart_request_t *obj = NULL;
    of_version_t version;
    int length;

    if (msg == NULL) return NULL;

    version = of_message_version_get(msg);
    if (!OF_VERSION_OKAY(version)) return NULL;

    length = of_message_length_get(msg);

    if ((obj = (ofdpa_mpls_set_qos_action_multipart_request_t *)of_object_new(-1)) == NULL) {
        return NULL;
    }

    ofdpa_mpls_set_qos_action_multipart_request_init(obj, version, 0, 0);

    if ((of_object_buffer_bind((of_object_t *)obj, OF_MESSAGE_TO_BUFFER(msg),
                               length, OF_MESSAGE_FREE_FUNCTION)) < 0) {
       FREE(obj);
       return NULL;
    }
    obj->length = length;
    obj->version = version;

    return obj;
}

/**
 * Get xid from an object of type ofdpa_mpls_set_qos_action_multipart_request.
 * @param obj Pointer to an object of type ofdpa_mpls_set_qos_action_multipart_request.
 * @param xid Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
ofdpa_mpls_set_qos_action_multipart_request_xid_get(
    ofdpa_mpls_set_qos_action_multipart_request_t *obj,
    uint32_t *xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_SET_QOS_ACTION_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, xid);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set xid in an object of type ofdpa_mpls_set_qos_action_multipart_request.
 * @param obj Pointer to an object of type ofdpa_mpls_set_qos_action_multipart_request.
 * @param xid The value to write into the object
 */
void
ofdpa_mpls_set_qos_action_multipart_request_xid_set(
    ofdpa_mpls_set_qos_action_multipart_request_t *obj,
    uint32_t xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_SET_QOS_ACTION_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, xid);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get flags from an object of type ofdpa_mpls_set_qos_action_multipart_request.
 * @param obj Pointer to an object of type ofdpa_mpls_set_qos_action_multipart_request.
 * @param flags Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
ofdpa_mpls_set_qos_action_multipart_request_flags_get(
    ofdpa_mpls_set_qos_action_multipart_request_t *obj,
    uint16_t *flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_SET_QOS_ACTION_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 10;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_get(wbuf, abs_offset, flags);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set flags in an object of type ofdpa_mpls_set_qos_action_multipart_request.
 * @param obj Pointer to an object of type ofdpa_mpls_set_qos_action_multipart_request.
 * @param flags The value to write into the object
 */
void
ofdpa_mpls_set_qos_action_multipart_request_flags_set(
    ofdpa_mpls_set_qos_action_multipart_request_t *obj,
    uint16_t flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_SET_QOS_ACTION_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 10;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_set(wbuf, abs_offset, flags);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get experimenter from an object of type ofdpa_mpls_set_qos_action_multipart_request.
 * @param obj Pointer to an object of type ofdpa_mpls_set_qos_action_multipart_request.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
ofdpa_mpls_set_qos_action_multipart_request_experimenter_get(
    ofdpa_mpls_set_qos_action_multipart_request_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_SET_QOS_ACTION_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 16;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set experimenter in an object of type ofdpa_mpls_set_qos_action_multipart_request.
 * @param obj Pointer to an object of type ofdpa_mpls_set_qos_action_multipart_request.
 * @param experimenter The value to write into the object
 */
void
ofdpa_mpls_set_qos_action_multipart_request_experimenter_set(
    ofdpa_mpls_set_qos_action_multipart_request_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_SET_QOS_ACTION_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 16;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get subtype from an object of type ofdpa_mpls_set_qos_action_multipart_request.
 * @param obj Pointer to an object of type ofdpa_mpls_set_qos_action_multipart_request.
 * @param subtype Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
ofdpa_mpls_set_qos_action_multipart_request_subtype_get(
    ofdpa_mpls_set_qos_action_multipart_request_t *obj,
    uint32_t *subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_SET_QOS_ACTION_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 20;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set subtype in an object of type ofdpa_mpls_set_qos_action_multipart_request.
 * @param obj Pointer to an object of type ofdpa_mpls_set_qos_action_multipart_request.
 * @param subtype The value to write into the object
 */
void
ofdpa_mpls_set_qos_action_multipart_request_subtype_set(
    ofdpa_mpls_set_qos_action_multipart_request_t *obj,
    uint32_t subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_SET_QOS_ACTION_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 20;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get qos_index from an object of type ofdpa_mpls_set_qos_action_multipart_request.
 * @param obj Pointer to an object of type ofdpa_mpls_set_qos_action_multipart_request.
 * @param qos_index Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
ofdpa_mpls_set_qos_action_multipart_request_qos_index_get(
    ofdpa_mpls_set_qos_action_multipart_request_t *obj,
    uint8_t *qos_index)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_SET_QOS_ACTION_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 24;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_get(wbuf, abs_offset, qos_index);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set qos_index in an object of type ofdpa_mpls_set_qos_action_multipart_request.
 * @param obj Pointer to an object of type ofdpa_mpls_set_qos_action_multipart_request.
 * @param qos_index The value to write into the object
 */
void
ofdpa_mpls_set_qos_action_multipart_request_qos_index_set(
    ofdpa_mpls_set_qos_action_multipart_request_t *obj,
    uint8_t qos_index)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_SET_QOS_ACTION_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 24;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_set(wbuf, abs_offset, qos_index);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get mpls_tc from an object of type ofdpa_mpls_set_qos_action_multipart_request.
 * @param obj Pointer to an object of type ofdpa_mpls_set_qos_action_multipart_request.
 * @param mpls_tc Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
ofdpa_mpls_set_qos_action_multipart_request_mpls_tc_get(
    ofdpa_mpls_set_qos_action_multipart_request_t *obj,
    uint8_t *mpls_tc)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_SET_QOS_ACTION_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 25;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_get(wbuf, abs_offset, mpls_tc);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set mpls_tc in an object of type ofdpa_mpls_set_qos_action_multipart_request.
 * @param obj Pointer to an object of type ofdpa_mpls_set_qos_action_multipart_request.
 * @param mpls_tc The value to write into the object
 */
void
ofdpa_mpls_set_qos_action_multipart_request_mpls_tc_set(
    ofdpa_mpls_set_qos_action_multipart_request_t *obj,
    uint8_t mpls_tc)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_SET_QOS_ACTION_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 25;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_set(wbuf, abs_offset, mpls_tc);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get traffic_class from an object of type ofdpa_mpls_set_qos_action_multipart_request.
 * @param obj Pointer to an object of type ofdpa_mpls_set_qos_action_multipart_request.
 * @param traffic_class Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
ofdpa_mpls_set_qos_action_multipart_request_traffic_class_get(
    ofdpa_mpls_set_qos_action_multipart_request_t *obj,
    uint8_t *traffic_class)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_SET_QOS_ACTION_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 26;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_get(wbuf, abs_offset, traffic_class);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set traffic_class in an object of type ofdpa_mpls_set_qos_action_multipart_request.
 * @param obj Pointer to an object of type ofdpa_mpls_set_qos_action_multipart_request.
 * @param traffic_class The value to write into the object
 */
void
ofdpa_mpls_set_qos_action_multipart_request_traffic_class_set(
    ofdpa_mpls_set_qos_action_multipart_request_t *obj,
    uint8_t traffic_class)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_SET_QOS_ACTION_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 26;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_set(wbuf, abs_offset, traffic_class);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get color from an object of type ofdpa_mpls_set_qos_action_multipart_request.
 * @param obj Pointer to an object of type ofdpa_mpls_set_qos_action_multipart_request.
 * @param color Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
ofdpa_mpls_set_qos_action_multipart_request_color_get(
    ofdpa_mpls_set_qos_action_multipart_request_t *obj,
    uint8_t *color)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_SET_QOS_ACTION_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 27;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_get(wbuf, abs_offset, color);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set color in an object of type ofdpa_mpls_set_qos_action_multipart_request.
 * @param obj Pointer to an object of type ofdpa_mpls_set_qos_action_multipart_request.
 * @param color The value to write into the object
 */
void
ofdpa_mpls_set_qos_action_multipart_request_color_set(
    ofdpa_mpls_set_qos_action_multipart_request_t *obj,
    uint8_t color)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_SET_QOS_ACTION_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 27;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_set(wbuf, abs_offset, color);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}
/* Copyright (c) 2008 The Board of Trustees of The Leland Stanford Junior University */
/* Copyright (c) 2011, 2012 Open Networking Foundation */
/* Copyright (c) 2012, 2013 Big Switch Networks, Inc. */
/* See the file LICENSE.loci which should have been included in the source distribution */
#ifdef __GNUC__

#ifdef __linux__
/* glibc */
#include <features.h>
#else
/* NetBSD etc */
#include <sys/cdefs.h>
#ifdef __GNUC_PREREQ__
#define __GNUC_PREREQ __GNUC_PREREQ__
#endif
#endif

#ifndef __GNUC_PREREQ
/* fallback */
#define __GNUC_PREREQ(maj, min) 0
#endif

#if __GNUC_PREREQ(4,6)
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif

#endif

#include "loci_log.h"
#include "loci_int.h"

static void
ofdpa_mpls_tunnel_label_multipart_reply_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint8_t *)(buf + 0) = obj->version; /* version */
        *(uint8_t *)(buf + 1) = 0x13; /* type */
        *(uint16_t *)(buf + 8) = U16_HTON(0xffff); /* stats_type */
        *(uint32_t *)(buf + 16) = U32_HTON(0x1018); /* experimenter */
        *(uint32_t *)(buf + 20) = U32_HTON(0x9); /* subtype */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup ofdpa_mpls_tunnel_label_multipart_reply ofdpa_mpls_tunnel_label_multipart_reply
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
ofdpa_mpls_tunnel_label_multipart_reply_push_wire_values(ofdpa_mpls_tunnel_label_multipart_reply_t *obj)
{

    ofdpa_mpls_tunnel_label_multipart_reply_push_wire_types(obj);

    /* Message obj; set length */
    of_message_t msg;

    if ((msg = OF_OBJECT_TO_MESSAGE(obj)) != NULL) {
        of_message_length_set(msg, obj->length);
    }

    return OF_ERROR_NONE;
}

/**
 * Create a new ofdpa_mpls_tunnel_label_multipart_reply object
 *
 * @param version The wire version to use for the object
 * @return Pointer to the newly create object or NULL on error
 *
 * Initializes the new object with it's default fixed length associating
 * a new underlying wire buffer.
 *
 * Use new_from_message to bind an existing message to a message object,
 * or a _get function for non-message objects.
 *
 * \ingroup ofdpa_mpls_tunnel_label_multipart_reply
 */

ofdpa_mpls_tunnel_label_multipart_reply_t *
ofdpa_mpls_tunnel_label_multipart_reply_new(of_version_t version)
{
    ofdpa_mpls_tunnel_label_multipart_reply_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OFDPA_MPLS_TUNNEL_LABEL_MULTIPART_REPLY] + of_object_extra_len[version][OFDPA_MPLS_TUNNEL_LABEL_MULTIPART_REPLY];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (ofdpa_mpls_tunnel_label_multipart_reply_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    ofdpa_mpls_tunnel_label_multipart_reply_init(obj, version, bytes, 0);

    if (ofdpa_mpls_tunnel_label_multipart_reply_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type ofdpa_mpls_tunnel_label_multipart_reply.
 *
 * @param obj Pointer to the object to initialize
 * @param version The wire version to use for the object
 * @param bytes How many bytes in the object
 * @param clean_wire Boolean: If true, clear the wire object control struct
 *
 * If bytes < 0, then the default fixed length is used for the object
 *
 * This is a "coerce" function that sets up the pointers for the
 * accessors properly.
 *
 * If anything other than 0 is passed in for the buffer size, the underlying
 * wire buffer will have 'grow' called.
 */

void
ofdpa_mpls_tunnel_label_multipart_reply_init(ofdpa_mpls_tunnel_label_multipart_reply_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OFDPA_MPLS_TUNNEL_LABEL_MULTIPART_REPLY] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OFDPA_MPLS_TUNNEL_LABEL_MULTIPART_REPLY] + of_object_extra_len[version][OFDPA_MPLS_TUNNEL_LABEL_MULTIPART_REPLY];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OFDPA_MPLS_TUNNEL_LABEL_MULTIPART_REPLY;

    /* Set up the object's function pointers */

    obj->wire_type_set = ofdpa_mpls_tunnel_label_multipart_reply_push_wire_types;

    obj->wire_length_get = of_object_message_wire_length_get;
    obj->wire_length_set = of_object_message_wire_length_set;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Create a new ofdpa_mpls_tunnel_label_multipart_reply object and bind it to an existing message
 *
 * @param msg The message to bind the new object to
 * @return Pointer to the newly create object or NULL on error
 *
 * \ingroup ofdpa_mpls_tunnel_label_multipart_reply
 */

ofdpa_mpls_tunnel_label_multipart_reply_t *
ofdpa_mpls_tunnel_label_multipart_reply_new_from_message(of_message_t msg)
{
    ofdpa_mpls_tunnel_label_multipart_reply_t *obj = NULL;
    of_version_t version;
    int length;

    if (msg == NULL) return NULL;

    version = of_message_version_get(msg);
    if (!OF_VERSION_OKAY(version)) return NULL;

    length = of_message_length_get(msg);

    if ((obj = (ofdpa_mpls_tunnel_label_multipart_reply_t *)of_object_new(-1)) == NULL) {
        return NULL;
    }

    ofdpa_mpls_tunnel_label_multipart_reply_init(obj, version, 0, 0);

    if ((of_object_buffer_bind((of_object_t *)obj, OF_MESSAGE_TO_BUFFER(msg),
                               length, OF_MESSAGE_FREE_FUNCTION)) < 0) {
       FREE(obj);
       return NULL;
    }
    obj->length = length;
    obj->version = version;

    return obj;
}

/**
 * Get xid from an object of type ofdpa_mpls_tunnel_label_multipart_reply.
 * @param obj Pointer to an object of type ofdpa_mpls_tunnel_label_multipart_reply.
 * @param xid Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
ofdpa_mpls_tunnel_label_multipart_reply_xid_get(
    ofdpa_mpls_tunnel_label_multipart_reply_t *obj,
    uint32_t *xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_TUNNEL_LABEL_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, xid);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set xid in an object of type ofdpa_mpls_tunnel_label_multipart_reply.
 * @param obj Pointer to an object of type ofdpa_mpls_tunnel_label_multipart_reply.
 * @param xid The value to write into the object
 */
void
ofdpa_mpls_tunnel_label_multipart_reply_xid_set(
    ofdpa_mpls_tunnel_label_multipart_reply_t *obj,
    uint32_t xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_TUNNEL_LABEL_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, xid);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get flags from an object of type ofdpa_mpls_tunnel_label_multipart_reply.
 * @param obj Pointer to an object of type ofdpa_mpls_tunnel_label_multipart_reply.
 * @param flags Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
ofdpa_mpls_tunnel_label_multipart_reply_flags_get(
    ofdpa_mpls_tunnel_label_multipart_reply_t *obj,
    uint16_t *flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_TUNNEL_LABEL_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 10;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_get(wbuf, abs_offset, flags);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set flags in an object of type ofdpa_mpls_tunnel_label_multipart_reply.
 * @param obj Pointer to an object of type ofdpa_mpls_tunnel_label_multipart_reply.
 * @param flags The value to write into the object
 */
void
ofdpa_mpls_tunnel_label_multipart_reply_flags_set(
    ofdpa_mpls_tunnel_label_multipart_reply_t *obj,
    uint16_t flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_TUNNEL_LABEL_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 10;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_set(wbuf, abs_offset, flags);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get experimenter from an object of type ofdpa_mpls_tunnel_label_multipart_reply.
 * @param obj Pointer to an object of type ofdpa_mpls_tunnel_label_multipart_reply.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
ofdpa_mpls_tunnel_label_multipart_reply_experimenter_get(
    ofdpa_mpls_tunnel_label_multipart_reply_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_TUNNEL_LABEL_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 16;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set experimenter in an object of type ofdpa_mpls_tunnel_label_multipart_reply.
 * @param obj Pointer to an object of type ofdpa_mpls_tunnel_label_multipart_reply.
 * @param experimenter The value to write into the object
 */
void
ofdpa_mpls_tunnel_label_multipart_reply_experimenter_set(
    ofdpa_mpls_tunnel_label_multipart_reply_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_TUNNEL_LABEL_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 16;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get subtype from an object of type ofdpa_mpls_tunnel_label_multipart_reply.
 * @param obj Pointer to an object of type ofdpa_mpls_tunnel_label_multipart_reply.
 * @param subtype Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
ofdpa_mpls_tunnel_label_multipart_reply_subtype_get(
    ofdpa_mpls_tunnel_label_multipart_reply_t *obj,
    uint32_t *subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_TUNNEL_LABEL_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 20;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set subtype in an object of type ofdpa_mpls_tunnel_label_multipart_reply.
 * @param obj Pointer to an object of type ofdpa_mpls_tunnel_label_multipart_reply.
 * @param subtype The value to write into the object
 */
void
ofdpa_mpls_tunnel_label_multipart_reply_subtype_set(
    ofdpa_mpls_tunnel_label_multipart_reply_t *obj,
    uint32_t subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_TUNNEL_LABEL_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 20;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get index from an object of type ofdpa_mpls_tunnel_label_multipart_reply.
 * @param obj Pointer to an object of type ofdpa_mpls_tunnel_label_multipart_reply.
 * @param index Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
ofdpa_mpls_tunnel_label_multipart_reply_index_get(
    ofdpa_mpls_tunnel_label_multipart_reply_t *obj,
    uint32_t *index)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_TUNNEL_LABEL_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 24;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, index);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set index in an object of type ofdpa_mpls_tunnel_label_multipart_reply.
 * @param obj Pointer to an object of type ofdpa_mpls_tunnel_label_multipart_reply.
 * @param index The value to write into the object
 */
void
ofdpa_mpls_tunnel_label_multipart_reply_index_set(
    ofdpa_mpls_tunnel_label_multipart_reply_t *obj,
    uint32_t index)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_TUNNEL_LABEL_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 24;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, index);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get traffic_class from an object of type ofdpa_mpls_tunnel_label_multipart_reply.
 * @param obj Pointer to an object of type ofdpa_mpls_tunnel_label_multipart_reply.
 * @param traffic_class Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
ofdpa_mpls_tunnel_label_multipart_reply_traffic_class_get(
    ofdpa_mpls_tunnel_label_multipart_reply_t *obj,
    uint8_t *traffic_class)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_TUNNEL_LABEL_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 28;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_get(wbuf, abs_offset, traffic_class);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set traffic_class in an object of type ofdpa_mpls_tunnel_label_multipart_reply.
 * @param obj Pointer to an object of type ofdpa_mpls_tunnel_label_multipart_reply.
 * @param traffic_class The value to write into the object
 */
void
ofdpa_mpls_tunnel_label_multipart_reply_traffic_class_set(
    ofdpa_mpls_tunnel_label_multipart_reply_t *obj,
    uint8_t traffic_class)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_TUNNEL_LABEL_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 28;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_set(wbuf, abs_offset, traffic_class);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get color from an object of type ofdpa_mpls_tunnel_label_multipart_reply.
 * @param obj Pointer to an object of type ofdpa_mpls_tunnel_label_multipart_reply.
 * @param color Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
ofdpa_mpls_tunnel_label_multipart_reply_color_get(
    ofdpa_mpls_tunnel_label_multipart_reply_t *obj,
    uint8_t *color)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_TUNNEL_LABEL_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 29;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_get(wbuf, abs_offset, color);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set color in an object of type ofdpa_mpls_tunnel_label_multipart_reply.
 * @param obj Pointer to an object of type ofdpa_mpls_tunnel_label_multipart_reply.
 * @param color The value to write into the object
 */
void
ofdpa_mpls_tunnel_label_multipart_reply_color_set(
    ofdpa_mpls_tunnel_label_multipart_reply_t *obj,
    uint8_t color)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_TUNNEL_LABEL_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 29;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_set(wbuf, abs_offset, color);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get mpls_tc from an object of type ofdpa_mpls_tunnel_label_multipart_reply.
 * @param obj Pointer to an object of type ofdpa_mpls_tunnel_label_multipart_reply.
 * @param mpls_tc Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
ofdpa_mpls_tunnel_label_multipart_reply_mpls_tc_get(
    ofdpa_mpls_tunnel_label_multipart_reply_t *obj,
    uint8_t *mpls_tc)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_TUNNEL_LABEL_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 30;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_get(wbuf, abs_offset, mpls_tc);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set mpls_tc in an object of type ofdpa_mpls_tunnel_label_multipart_reply.
 * @param obj Pointer to an object of type ofdpa_mpls_tunnel_label_multipart_reply.
 * @param mpls_tc The value to write into the object
 */
void
ofdpa_mpls_tunnel_label_multipart_reply_mpls_tc_set(
    ofdpa_mpls_tunnel_label_multipart_reply_t *obj,
    uint8_t mpls_tc)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_TUNNEL_LABEL_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 30;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_set(wbuf, abs_offset, mpls_tc);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get vlan_pcp from an object of type ofdpa_mpls_tunnel_label_multipart_reply.
 * @param obj Pointer to an object of type ofdpa_mpls_tunnel_label_multipart_reply.
 * @param vlan_pcp Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
ofdpa_mpls_tunnel_label_multipart_reply_vlan_pcp_get(
    ofdpa_mpls_tunnel_label_multipart_reply_t *obj,
    uint8_t *vlan_pcp)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_TUNNEL_LABEL_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 31;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_get(wbuf, abs_offset, vlan_pcp);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set vlan_pcp in an object of type ofdpa_mpls_tunnel_label_multipart_reply.
 * @param obj Pointer to an object of type ofdpa_mpls_tunnel_label_multipart_reply.
 * @param vlan_pcp The value to write into the object
 */
void
ofdpa_mpls_tunnel_label_multipart_reply_vlan_pcp_set(
    ofdpa_mpls_tunnel_label_multipart_reply_t *obj,
    uint8_t vlan_pcp)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_TUNNEL_LABEL_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 31;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_set(wbuf, abs_offset, vlan_pcp);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get vlan_dei from an object of type ofdpa_mpls_tunnel_label_multipart_reply.
 * @param obj Pointer to an object of type ofdpa_mpls_tunnel_label_multipart_reply.
 * @param vlan_dei Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
ofdpa_mpls_tunnel_label_multipart_reply_vlan_dei_get(
    ofdpa_mpls_tunnel_label_multipart_reply_t *obj,
    uint8_t *vlan_dei)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_TUNNEL_LABEL_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 32;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_get(wbuf, abs_offset, vlan_dei);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set vlan_dei in an object of type ofdpa_mpls_tunnel_label_multipart_reply.
 * @param obj Pointer to an object of type ofdpa_mpls_tunnel_label_multipart_reply.
 * @param vlan_dei The value to write into the object
 */
void
ofdpa_mpls_tunnel_label_multipart_reply_vlan_dei_set(
    ofdpa_mpls_tunnel_label_multipart_reply_t *obj,
    uint8_t vlan_dei)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_TUNNEL_LABEL_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 32;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_set(wbuf, abs_offset, vlan_dei);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}
/* Copyright (c) 2008 The Board of Trustees of The Leland Stanford Junior University */
/* Copyright (c) 2011, 2012 Open Networking Foundation */
/* Copyright (c) 2012, 2013 Big Switch Networks, Inc. */
/* See the file LICENSE.loci which should have been included in the source distribution */
#ifdef __GNUC__

#ifdef __linux__
/* glibc */
#include <features.h>
#else
/* NetBSD etc */
#include <sys/cdefs.h>
#ifdef __GNUC_PREREQ__
#define __GNUC_PREREQ __GNUC_PREREQ__
#endif
#endif

#ifndef __GNUC_PREREQ
/* fallback */
#define __GNUC_PREREQ(maj, min) 0
#endif

#if __GNUC_PREREQ(4,6)
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif

#endif

#include "loci_log.h"
#include "loci_int.h"

static void
ofdpa_mpls_tunnel_label_multipart_request_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint8_t *)(buf + 0) = obj->version; /* version */
        *(uint8_t *)(buf + 1) = 0x12; /* type */
        *(uint16_t *)(buf + 8) = U16_HTON(0xffff); /* stats_type */
        *(uint32_t *)(buf + 16) = U32_HTON(0x1018); /* experimenter */
        *(uint32_t *)(buf + 20) = U32_HTON(0x9); /* subtype */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup ofdpa_mpls_tunnel_label_multipart_request ofdpa_mpls_tunnel_label_multipart_request
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
ofdpa_mpls_tunnel_label_multipart_request_push_wire_values(ofdpa_mpls_tunnel_label_multipart_request_t *obj)
{

    ofdpa_mpls_tunnel_label_multipart_request_push_wire_types(obj);

    /* Message obj; set length */
    of_message_t msg;

    if ((msg = OF_OBJECT_TO_MESSAGE(obj)) != NULL) {
        of_message_length_set(msg, obj->length);
    }

    return OF_ERROR_NONE;
}

/**
 * Create a new ofdpa_mpls_tunnel_label_multipart_request object
 *
 * @param version The wire version to use for the object
 * @return Pointer to the newly create object or NULL on error
 *
 * Initializes the new object with it's default fixed length associating
 * a new underlying wire buffer.
 *
 * Use new_from_message to bind an existing message to a message object,
 * or a _get function for non-message objects.
 *
 * \ingroup ofdpa_mpls_tunnel_label_multipart_request
 */

ofdpa_mpls_tunnel_label_multipart_request_t *
ofdpa_mpls_tunnel_label_multipart_request_new(of_version_t version)
{
    ofdpa_mpls_tunnel_label_multipart_request_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OFDPA_MPLS_TUNNEL_LABEL_MULTIPART_REQUEST] + of_object_extra_len[version][OFDPA_MPLS_TUNNEL_LABEL_MULTIPART_REQUEST];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (ofdpa_mpls_tunnel_label_multipart_request_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    ofdpa_mpls_tunnel_label_multipart_request_init(obj, version, bytes, 0);

    if (ofdpa_mpls_tunnel_label_multipart_request_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type ofdpa_mpls_tunnel_label_multipart_request.
 *
 * @param obj Pointer to the object to initialize
 * @param version The wire version to use for the object
 * @param bytes How many bytes in the object
 * @param clean_wire Boolean: If true, clear the wire object control struct
 *
 * If bytes < 0, then the default fixed length is used for the object
 *
 * This is a "coerce" function that sets up the pointers for the
 * accessors properly.
 *
 * If anything other than 0 is passed in for the buffer size, the underlying
 * wire buffer will have 'grow' called.
 */

void
ofdpa_mpls_tunnel_label_multipart_request_init(ofdpa_mpls_tunnel_label_multipart_request_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OFDPA_MPLS_TUNNEL_LABEL_MULTIPART_REQUEST] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OFDPA_MPLS_TUNNEL_LABEL_MULTIPART_REQUEST] + of_object_extra_len[version][OFDPA_MPLS_TUNNEL_LABEL_MULTIPART_REQUEST];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OFDPA_MPLS_TUNNEL_LABEL_MULTIPART_REQUEST;

    /* Set up the object's function pointers */

    obj->wire_type_set = ofdpa_mpls_tunnel_label_multipart_request_push_wire_types;

    obj->wire_length_get = of_object_message_wire_length_get;
    obj->wire_length_set = of_object_message_wire_length_set;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Create a new ofdpa_mpls_tunnel_label_multipart_request object and bind it to an existing message
 *
 * @param msg The message to bind the new object to
 * @return Pointer to the newly create object or NULL on error
 *
 * \ingroup ofdpa_mpls_tunnel_label_multipart_request
 */

ofdpa_mpls_tunnel_label_multipart_request_t *
ofdpa_mpls_tunnel_label_multipart_request_new_from_message(of_message_t msg)
{
    ofdpa_mpls_tunnel_label_multipart_request_t *obj = NULL;
    of_version_t version;
    int length;

    if (msg == NULL) return NULL;

    version = of_message_version_get(msg);
    if (!OF_VERSION_OKAY(version)) return NULL;

    length = of_message_length_get(msg);

    if ((obj = (ofdpa_mpls_tunnel_label_multipart_request_t *)of_object_new(-1)) == NULL) {
        return NULL;
    }

    ofdpa_mpls_tunnel_label_multipart_request_init(obj, version, 0, 0);

    if ((of_object_buffer_bind((of_object_t *)obj, OF_MESSAGE_TO_BUFFER(msg),
                               length, OF_MESSAGE_FREE_FUNCTION)) < 0) {
       FREE(obj);
       return NULL;
    }
    obj->length = length;
    obj->version = version;

    return obj;
}

/**
 * Get xid from an object of type ofdpa_mpls_tunnel_label_multipart_request.
 * @param obj Pointer to an object of type ofdpa_mpls_tunnel_label_multipart_request.
 * @param xid Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
ofdpa_mpls_tunnel_label_multipart_request_xid_get(
    ofdpa_mpls_tunnel_label_multipart_request_t *obj,
    uint32_t *xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_TUNNEL_LABEL_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, xid);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set xid in an object of type ofdpa_mpls_tunnel_label_multipart_request.
 * @param obj Pointer to an object of type ofdpa_mpls_tunnel_label_multipart_request.
 * @param xid The value to write into the object
 */
void
ofdpa_mpls_tunnel_label_multipart_request_xid_set(
    ofdpa_mpls_tunnel_label_multipart_request_t *obj,
    uint32_t xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_TUNNEL_LABEL_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, xid);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get flags from an object of type ofdpa_mpls_tunnel_label_multipart_request.
 * @param obj Pointer to an object of type ofdpa_mpls_tunnel_label_multipart_request.
 * @param flags Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
ofdpa_mpls_tunnel_label_multipart_request_flags_get(
    ofdpa_mpls_tunnel_label_multipart_request_t *obj,
    uint16_t *flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_TUNNEL_LABEL_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 10;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_get(wbuf, abs_offset, flags);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set flags in an object of type ofdpa_mpls_tunnel_label_multipart_request.
 * @param obj Pointer to an object of type ofdpa_mpls_tunnel_label_multipart_request.
 * @param flags The value to write into the object
 */
void
ofdpa_mpls_tunnel_label_multipart_request_flags_set(
    ofdpa_mpls_tunnel_label_multipart_request_t *obj,
    uint16_t flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_TUNNEL_LABEL_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 10;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_set(wbuf, abs_offset, flags);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get experimenter from an object of type ofdpa_mpls_tunnel_label_multipart_request.
 * @param obj Pointer to an object of type ofdpa_mpls_tunnel_label_multipart_request.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
ofdpa_mpls_tunnel_label_multipart_request_experimenter_get(
    ofdpa_mpls_tunnel_label_multipart_request_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_TUNNEL_LABEL_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 16;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set experimenter in an object of type ofdpa_mpls_tunnel_label_multipart_request.
 * @param obj Pointer to an object of type ofdpa_mpls_tunnel_label_multipart_request.
 * @param experimenter The value to write into the object
 */
void
ofdpa_mpls_tunnel_label_multipart_request_experimenter_set(
    ofdpa_mpls_tunnel_label_multipart_request_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_TUNNEL_LABEL_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 16;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get subtype from an object of type ofdpa_mpls_tunnel_label_multipart_request.
 * @param obj Pointer to an object of type ofdpa_mpls_tunnel_label_multipart_request.
 * @param subtype Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
ofdpa_mpls_tunnel_label_multipart_request_subtype_get(
    ofdpa_mpls_tunnel_label_multipart_request_t *obj,
    uint32_t *subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_TUNNEL_LABEL_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 20;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set subtype in an object of type ofdpa_mpls_tunnel_label_multipart_request.
 * @param obj Pointer to an object of type ofdpa_mpls_tunnel_label_multipart_request.
 * @param subtype The value to write into the object
 */
void
ofdpa_mpls_tunnel_label_multipart_request_subtype_set(
    ofdpa_mpls_tunnel_label_multipart_request_t *obj,
    uint32_t subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_TUNNEL_LABEL_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 20;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get index from an object of type ofdpa_mpls_tunnel_label_multipart_request.
 * @param obj Pointer to an object of type ofdpa_mpls_tunnel_label_multipart_request.
 * @param index Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
ofdpa_mpls_tunnel_label_multipart_request_index_get(
    ofdpa_mpls_tunnel_label_multipart_request_t *obj,
    uint32_t *index)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_TUNNEL_LABEL_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 24;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, index);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set index in an object of type ofdpa_mpls_tunnel_label_multipart_request.
 * @param obj Pointer to an object of type ofdpa_mpls_tunnel_label_multipart_request.
 * @param index The value to write into the object
 */
void
ofdpa_mpls_tunnel_label_multipart_request_index_set(
    ofdpa_mpls_tunnel_label_multipart_request_t *obj,
    uint32_t index)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_TUNNEL_LABEL_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 24;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, index);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get traffic_class from an object of type ofdpa_mpls_tunnel_label_multipart_request.
 * @param obj Pointer to an object of type ofdpa_mpls_tunnel_label_multipart_request.
 * @param traffic_class Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
ofdpa_mpls_tunnel_label_multipart_request_traffic_class_get(
    ofdpa_mpls_tunnel_label_multipart_request_t *obj,
    uint8_t *traffic_class)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_TUNNEL_LABEL_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 28;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_get(wbuf, abs_offset, traffic_class);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set traffic_class in an object of type ofdpa_mpls_tunnel_label_multipart_request.
 * @param obj Pointer to an object of type ofdpa_mpls_tunnel_label_multipart_request.
 * @param traffic_class The value to write into the object
 */
void
ofdpa_mpls_tunnel_label_multipart_request_traffic_class_set(
    ofdpa_mpls_tunnel_label_multipart_request_t *obj,
    uint8_t traffic_class)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_TUNNEL_LABEL_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 28;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_set(wbuf, abs_offset, traffic_class);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get color from an object of type ofdpa_mpls_tunnel_label_multipart_request.
 * @param obj Pointer to an object of type ofdpa_mpls_tunnel_label_multipart_request.
 * @param color Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
ofdpa_mpls_tunnel_label_multipart_request_color_get(
    ofdpa_mpls_tunnel_label_multipart_request_t *obj,
    uint8_t *color)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_TUNNEL_LABEL_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 29;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_get(wbuf, abs_offset, color);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set color in an object of type ofdpa_mpls_tunnel_label_multipart_request.
 * @param obj Pointer to an object of type ofdpa_mpls_tunnel_label_multipart_request.
 * @param color The value to write into the object
 */
void
ofdpa_mpls_tunnel_label_multipart_request_color_set(
    ofdpa_mpls_tunnel_label_multipart_request_t *obj,
    uint8_t color)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_TUNNEL_LABEL_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 29;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_set(wbuf, abs_offset, color);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get mpls_tc from an object of type ofdpa_mpls_tunnel_label_multipart_request.
 * @param obj Pointer to an object of type ofdpa_mpls_tunnel_label_multipart_request.
 * @param mpls_tc Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
ofdpa_mpls_tunnel_label_multipart_request_mpls_tc_get(
    ofdpa_mpls_tunnel_label_multipart_request_t *obj,
    uint8_t *mpls_tc)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_TUNNEL_LABEL_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 30;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_get(wbuf, abs_offset, mpls_tc);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set mpls_tc in an object of type ofdpa_mpls_tunnel_label_multipart_request.
 * @param obj Pointer to an object of type ofdpa_mpls_tunnel_label_multipart_request.
 * @param mpls_tc The value to write into the object
 */
void
ofdpa_mpls_tunnel_label_multipart_request_mpls_tc_set(
    ofdpa_mpls_tunnel_label_multipart_request_t *obj,
    uint8_t mpls_tc)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_TUNNEL_LABEL_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 30;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_set(wbuf, abs_offset, mpls_tc);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get vlan_pcp from an object of type ofdpa_mpls_tunnel_label_multipart_request.
 * @param obj Pointer to an object of type ofdpa_mpls_tunnel_label_multipart_request.
 * @param vlan_pcp Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
ofdpa_mpls_tunnel_label_multipart_request_vlan_pcp_get(
    ofdpa_mpls_tunnel_label_multipart_request_t *obj,
    uint8_t *vlan_pcp)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_TUNNEL_LABEL_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 31;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_get(wbuf, abs_offset, vlan_pcp);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set vlan_pcp in an object of type ofdpa_mpls_tunnel_label_multipart_request.
 * @param obj Pointer to an object of type ofdpa_mpls_tunnel_label_multipart_request.
 * @param vlan_pcp The value to write into the object
 */
void
ofdpa_mpls_tunnel_label_multipart_request_vlan_pcp_set(
    ofdpa_mpls_tunnel_label_multipart_request_t *obj,
    uint8_t vlan_pcp)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_TUNNEL_LABEL_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 31;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_set(wbuf, abs_offset, vlan_pcp);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get vlan_dei from an object of type ofdpa_mpls_tunnel_label_multipart_request.
 * @param obj Pointer to an object of type ofdpa_mpls_tunnel_label_multipart_request.
 * @param vlan_dei Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
ofdpa_mpls_tunnel_label_multipart_request_vlan_dei_get(
    ofdpa_mpls_tunnel_label_multipart_request_t *obj,
    uint8_t *vlan_dei)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_TUNNEL_LABEL_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 32;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_get(wbuf, abs_offset, vlan_dei);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set vlan_dei in an object of type ofdpa_mpls_tunnel_label_multipart_request.
 * @param obj Pointer to an object of type ofdpa_mpls_tunnel_label_multipart_request.
 * @param vlan_dei The value to write into the object
 */
void
ofdpa_mpls_tunnel_label_multipart_request_vlan_dei_set(
    ofdpa_mpls_tunnel_label_multipart_request_t *obj,
    uint8_t vlan_dei)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_TUNNEL_LABEL_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 32;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_set(wbuf, abs_offset, vlan_dei);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}
/* Copyright (c) 2008 The Board of Trustees of The Leland Stanford Junior University */
/* Copyright (c) 2011, 2012 Open Networking Foundation */
/* Copyright (c) 2012, 2013 Big Switch Networks, Inc. */
/* See the file LICENSE.loci which should have been included in the source distribution */
#ifdef __GNUC__

#ifdef __linux__
/* glibc */
#include <features.h>
#else
/* NetBSD etc */
#include <sys/cdefs.h>
#ifdef __GNUC_PREREQ__
#define __GNUC_PREREQ __GNUC_PREREQ__
#endif
#endif

#ifndef __GNUC_PREREQ
/* fallback */
#define __GNUC_PREREQ(maj, min) 0
#endif

#if __GNUC_PREREQ(4,6)
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif

#endif

#include "loci_log.h"
#include "loci_int.h"

static void
ofdpa_mpls_tunnel_label_remark_action_mod_msg_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint8_t *)(buf + 0) = obj->version; /* version */
        *(uint8_t *)(buf + 1) = 0x4; /* type */
        *(uint32_t *)(buf + 8) = U32_HTON(0x1018); /* experimenter */
        *(uint32_t *)(buf + 12) = U32_HTON(0x8); /* subtype */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup ofdpa_mpls_tunnel_label_remark_action_mod_msg ofdpa_mpls_tunnel_label_remark_action_mod_msg
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
ofdpa_mpls_tunnel_label_remark_action_mod_msg_push_wire_values(ofdpa_mpls_tunnel_label_remark_action_mod_msg_t *obj)
{

    ofdpa_mpls_tunnel_label_remark_action_mod_msg_push_wire_types(obj);

    /* Message obj; set length */
    of_message_t msg;

    if ((msg = OF_OBJECT_TO_MESSAGE(obj)) != NULL) {
        of_message_length_set(msg, obj->length);
    }

    return OF_ERROR_NONE;
}

/**
 * Create a new ofdpa_mpls_tunnel_label_remark_action_mod_msg object
 *
 * @param version The wire version to use for the object
 * @return Pointer to the newly create object or NULL on error
 *
 * Initializes the new object with it's default fixed length associating
 * a new underlying wire buffer.
 *
 * Use new_from_message to bind an existing message to a message object,
 * or a _get function for non-message objects.
 *
 * \ingroup ofdpa_mpls_tunnel_label_remark_action_mod_msg
 */

ofdpa_mpls_tunnel_label_remark_action_mod_msg_t *
ofdpa_mpls_tunnel_label_remark_action_mod_msg_new(of_version_t version)
{
    ofdpa_mpls_tunnel_label_remark_action_mod_msg_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OFDPA_MPLS_TUNNEL_LABEL_REMARK_ACTION_MOD_MSG] + of_object_extra_len[version][OFDPA_MPLS_TUNNEL_LABEL_REMARK_ACTION_MOD_MSG];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (ofdpa_mpls_tunnel_label_remark_action_mod_msg_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    ofdpa_mpls_tunnel_label_remark_action_mod_msg_init(obj, version, bytes, 0);

    if (ofdpa_mpls_tunnel_label_remark_action_mod_msg_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type ofdpa_mpls_tunnel_label_remark_action_mod_msg.
 *
 * @param obj Pointer to the object to initialize
 * @param version The wire version to use for the object
 * @param bytes How many bytes in the object
 * @param clean_wire Boolean: If true, clear the wire object control struct
 *
 * If bytes < 0, then the default fixed length is used for the object
 *
 * This is a "coerce" function that sets up the pointers for the
 * accessors properly.
 *
 * If anything other than 0 is passed in for the buffer size, the underlying
 * wire buffer will have 'grow' called.
 */

void
ofdpa_mpls_tunnel_label_remark_action_mod_msg_init(ofdpa_mpls_tunnel_label_remark_action_mod_msg_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OFDPA_MPLS_TUNNEL_LABEL_REMARK_ACTION_MOD_MSG] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OFDPA_MPLS_TUNNEL_LABEL_REMARK_ACTION_MOD_MSG] + of_object_extra_len[version][OFDPA_MPLS_TUNNEL_LABEL_REMARK_ACTION_MOD_MSG];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OFDPA_MPLS_TUNNEL_LABEL_REMARK_ACTION_MOD_MSG;

    /* Set up the object's function pointers */

    obj->wire_type_set = ofdpa_mpls_tunnel_label_remark_action_mod_msg_push_wire_types;

    obj->wire_length_get = of_object_message_wire_length_get;
    obj->wire_length_set = of_object_message_wire_length_set;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Create a new ofdpa_mpls_tunnel_label_remark_action_mod_msg object and bind it to an existing message
 *
 * @param msg The message to bind the new object to
 * @return Pointer to the newly create object or NULL on error
 *
 * \ingroup ofdpa_mpls_tunnel_label_remark_action_mod_msg
 */

ofdpa_mpls_tunnel_label_remark_action_mod_msg_t *
ofdpa_mpls_tunnel_label_remark_action_mod_msg_new_from_message(of_message_t msg)
{
    ofdpa_mpls_tunnel_label_remark_action_mod_msg_t *obj = NULL;
    of_version_t version;
    int length;

    if (msg == NULL) return NULL;

    version = of_message_version_get(msg);
    if (!OF_VERSION_OKAY(version)) return NULL;

    length = of_message_length_get(msg);

    if ((obj = (ofdpa_mpls_tunnel_label_remark_action_mod_msg_t *)of_object_new(-1)) == NULL) {
        return NULL;
    }

    ofdpa_mpls_tunnel_label_remark_action_mod_msg_init(obj, version, 0, 0);

    if ((of_object_buffer_bind((of_object_t *)obj, OF_MESSAGE_TO_BUFFER(msg),
                               length, OF_MESSAGE_FREE_FUNCTION)) < 0) {
       FREE(obj);
       return NULL;
    }
    obj->length = length;
    obj->version = version;

    return obj;
}

/**
 * Get xid from an object of type ofdpa_mpls_tunnel_label_remark_action_mod_msg.
 * @param obj Pointer to an object of type ofdpa_mpls_tunnel_label_remark_action_mod_msg.
 * @param xid Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
ofdpa_mpls_tunnel_label_remark_action_mod_msg_xid_get(
    ofdpa_mpls_tunnel_label_remark_action_mod_msg_t *obj,
    uint32_t *xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_TUNNEL_LABEL_REMARK_ACTION_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, xid);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set xid in an object of type ofdpa_mpls_tunnel_label_remark_action_mod_msg.
 * @param obj Pointer to an object of type ofdpa_mpls_tunnel_label_remark_action_mod_msg.
 * @param xid The value to write into the object
 */
void
ofdpa_mpls_tunnel_label_remark_action_mod_msg_xid_set(
    ofdpa_mpls_tunnel_label_remark_action_mod_msg_t *obj,
    uint32_t xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_TUNNEL_LABEL_REMARK_ACTION_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, xid);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get experimenter from an object of type ofdpa_mpls_tunnel_label_remark_action_mod_msg.
 * @param obj Pointer to an object of type ofdpa_mpls_tunnel_label_remark_action_mod_msg.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
ofdpa_mpls_tunnel_label_remark_action_mod_msg_experimenter_get(
    ofdpa_mpls_tunnel_label_remark_action_mod_msg_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_TUNNEL_LABEL_REMARK_ACTION_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 8;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set experimenter in an object of type ofdpa_mpls_tunnel_label_remark_action_mod_msg.
 * @param obj Pointer to an object of type ofdpa_mpls_tunnel_label_remark_action_mod_msg.
 * @param experimenter The value to write into the object
 */
void
ofdpa_mpls_tunnel_label_remark_action_mod_msg_experimenter_set(
    ofdpa_mpls_tunnel_label_remark_action_mod_msg_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_TUNNEL_LABEL_REMARK_ACTION_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 8;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get subtype from an object of type ofdpa_mpls_tunnel_label_remark_action_mod_msg.
 * @param obj Pointer to an object of type ofdpa_mpls_tunnel_label_remark_action_mod_msg.
 * @param subtype Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
ofdpa_mpls_tunnel_label_remark_action_mod_msg_subtype_get(
    ofdpa_mpls_tunnel_label_remark_action_mod_msg_t *obj,
    uint32_t *subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_TUNNEL_LABEL_REMARK_ACTION_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 12;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set subtype in an object of type ofdpa_mpls_tunnel_label_remark_action_mod_msg.
 * @param obj Pointer to an object of type ofdpa_mpls_tunnel_label_remark_action_mod_msg.
 * @param subtype The value to write into the object
 */
void
ofdpa_mpls_tunnel_label_remark_action_mod_msg_subtype_set(
    ofdpa_mpls_tunnel_label_remark_action_mod_msg_t *obj,
    uint32_t subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_TUNNEL_LABEL_REMARK_ACTION_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 12;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get mod_command from an object of type ofdpa_mpls_tunnel_label_remark_action_mod_msg.
 * @param obj Pointer to an object of type ofdpa_mpls_tunnel_label_remark_action_mod_msg.
 * @param mod_command Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
ofdpa_mpls_tunnel_label_remark_action_mod_msg_mod_command_get(
    ofdpa_mpls_tunnel_label_remark_action_mod_msg_t *obj,
    uint32_t *mod_command)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_TUNNEL_LABEL_REMARK_ACTION_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 16;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, mod_command);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set mod_command in an object of type ofdpa_mpls_tunnel_label_remark_action_mod_msg.
 * @param obj Pointer to an object of type ofdpa_mpls_tunnel_label_remark_action_mod_msg.
 * @param mod_command The value to write into the object
 */
void
ofdpa_mpls_tunnel_label_remark_action_mod_msg_mod_command_set(
    ofdpa_mpls_tunnel_label_remark_action_mod_msg_t *obj,
    uint32_t mod_command)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_TUNNEL_LABEL_REMARK_ACTION_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 16;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, mod_command);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get index from an object of type ofdpa_mpls_tunnel_label_remark_action_mod_msg.
 * @param obj Pointer to an object of type ofdpa_mpls_tunnel_label_remark_action_mod_msg.
 * @param index Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
ofdpa_mpls_tunnel_label_remark_action_mod_msg_index_get(
    ofdpa_mpls_tunnel_label_remark_action_mod_msg_t *obj,
    uint32_t *index)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_TUNNEL_LABEL_REMARK_ACTION_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 20;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, index);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set index in an object of type ofdpa_mpls_tunnel_label_remark_action_mod_msg.
 * @param obj Pointer to an object of type ofdpa_mpls_tunnel_label_remark_action_mod_msg.
 * @param index The value to write into the object
 */
void
ofdpa_mpls_tunnel_label_remark_action_mod_msg_index_set(
    ofdpa_mpls_tunnel_label_remark_action_mod_msg_t *obj,
    uint32_t index)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_TUNNEL_LABEL_REMARK_ACTION_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 20;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, index);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get traffic_class from an object of type ofdpa_mpls_tunnel_label_remark_action_mod_msg.
 * @param obj Pointer to an object of type ofdpa_mpls_tunnel_label_remark_action_mod_msg.
 * @param traffic_class Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
ofdpa_mpls_tunnel_label_remark_action_mod_msg_traffic_class_get(
    ofdpa_mpls_tunnel_label_remark_action_mod_msg_t *obj,
    uint8_t *traffic_class)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_TUNNEL_LABEL_REMARK_ACTION_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 24;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_get(wbuf, abs_offset, traffic_class);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set traffic_class in an object of type ofdpa_mpls_tunnel_label_remark_action_mod_msg.
 * @param obj Pointer to an object of type ofdpa_mpls_tunnel_label_remark_action_mod_msg.
 * @param traffic_class The value to write into the object
 */
void
ofdpa_mpls_tunnel_label_remark_action_mod_msg_traffic_class_set(
    ofdpa_mpls_tunnel_label_remark_action_mod_msg_t *obj,
    uint8_t traffic_class)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_TUNNEL_LABEL_REMARK_ACTION_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 24;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_set(wbuf, abs_offset, traffic_class);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get color from an object of type ofdpa_mpls_tunnel_label_remark_action_mod_msg.
 * @param obj Pointer to an object of type ofdpa_mpls_tunnel_label_remark_action_mod_msg.
 * @param color Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
ofdpa_mpls_tunnel_label_remark_action_mod_msg_color_get(
    ofdpa_mpls_tunnel_label_remark_action_mod_msg_t *obj,
    uint8_t *color)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_TUNNEL_LABEL_REMARK_ACTION_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 25;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_get(wbuf, abs_offset, color);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set color in an object of type ofdpa_mpls_tunnel_label_remark_action_mod_msg.
 * @param obj Pointer to an object of type ofdpa_mpls_tunnel_label_remark_action_mod_msg.
 * @param color The value to write into the object
 */
void
ofdpa_mpls_tunnel_label_remark_action_mod_msg_color_set(
    ofdpa_mpls_tunnel_label_remark_action_mod_msg_t *obj,
    uint8_t color)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_TUNNEL_LABEL_REMARK_ACTION_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 25;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_set(wbuf, abs_offset, color);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get mpls_tc from an object of type ofdpa_mpls_tunnel_label_remark_action_mod_msg.
 * @param obj Pointer to an object of type ofdpa_mpls_tunnel_label_remark_action_mod_msg.
 * @param mpls_tc Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
ofdpa_mpls_tunnel_label_remark_action_mod_msg_mpls_tc_get(
    ofdpa_mpls_tunnel_label_remark_action_mod_msg_t *obj,
    uint8_t *mpls_tc)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_TUNNEL_LABEL_REMARK_ACTION_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 26;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_get(wbuf, abs_offset, mpls_tc);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set mpls_tc in an object of type ofdpa_mpls_tunnel_label_remark_action_mod_msg.
 * @param obj Pointer to an object of type ofdpa_mpls_tunnel_label_remark_action_mod_msg.
 * @param mpls_tc The value to write into the object
 */
void
ofdpa_mpls_tunnel_label_remark_action_mod_msg_mpls_tc_set(
    ofdpa_mpls_tunnel_label_remark_action_mod_msg_t *obj,
    uint8_t mpls_tc)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_TUNNEL_LABEL_REMARK_ACTION_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 26;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_set(wbuf, abs_offset, mpls_tc);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get vlan_pcp from an object of type ofdpa_mpls_tunnel_label_remark_action_mod_msg.
 * @param obj Pointer to an object of type ofdpa_mpls_tunnel_label_remark_action_mod_msg.
 * @param vlan_pcp Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
ofdpa_mpls_tunnel_label_remark_action_mod_msg_vlan_pcp_get(
    ofdpa_mpls_tunnel_label_remark_action_mod_msg_t *obj,
    uint8_t *vlan_pcp)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_TUNNEL_LABEL_REMARK_ACTION_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 27;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_get(wbuf, abs_offset, vlan_pcp);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set vlan_pcp in an object of type ofdpa_mpls_tunnel_label_remark_action_mod_msg.
 * @param obj Pointer to an object of type ofdpa_mpls_tunnel_label_remark_action_mod_msg.
 * @param vlan_pcp The value to write into the object
 */
void
ofdpa_mpls_tunnel_label_remark_action_mod_msg_vlan_pcp_set(
    ofdpa_mpls_tunnel_label_remark_action_mod_msg_t *obj,
    uint8_t vlan_pcp)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_TUNNEL_LABEL_REMARK_ACTION_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 27;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_set(wbuf, abs_offset, vlan_pcp);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get vlan_dei from an object of type ofdpa_mpls_tunnel_label_remark_action_mod_msg.
 * @param obj Pointer to an object of type ofdpa_mpls_tunnel_label_remark_action_mod_msg.
 * @param vlan_dei Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
ofdpa_mpls_tunnel_label_remark_action_mod_msg_vlan_dei_get(
    ofdpa_mpls_tunnel_label_remark_action_mod_msg_t *obj,
    uint8_t *vlan_dei)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_TUNNEL_LABEL_REMARK_ACTION_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 28;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_get(wbuf, abs_offset, vlan_dei);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set vlan_dei in an object of type ofdpa_mpls_tunnel_label_remark_action_mod_msg.
 * @param obj Pointer to an object of type ofdpa_mpls_tunnel_label_remark_action_mod_msg.
 * @param vlan_dei The value to write into the object
 */
void
ofdpa_mpls_tunnel_label_remark_action_mod_msg_vlan_dei_set(
    ofdpa_mpls_tunnel_label_remark_action_mod_msg_t *obj,
    uint8_t vlan_dei)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_TUNNEL_LABEL_REMARK_ACTION_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 28;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_set(wbuf, abs_offset, vlan_dei);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}
/* Copyright (c) 2008 The Board of Trustees of The Leland Stanford Junior University */
/* Copyright (c) 2011, 2012 Open Networking Foundation */
/* Copyright (c) 2012, 2013 Big Switch Networks, Inc. */
/* See the file LICENSE.loci which should have been included in the source distribution */
#ifdef __GNUC__

#ifdef __linux__
/* glibc */
#include <features.h>
#else
/* NetBSD etc */
#include <sys/cdefs.h>
#ifdef __GNUC_PREREQ__
#define __GNUC_PREREQ __GNUC_PREREQ__
#endif
#endif

#ifndef __GNUC_PREREQ
/* fallback */
#define __GNUC_PREREQ(maj, min) 0
#endif

#if __GNUC_PREREQ(4,6)
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif

#endif

#include "loci_log.h"
#include "loci_int.h"

static void
ofdpa_mpls_vpn_label_remark_action_mod_msg_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint8_t *)(buf + 0) = obj->version; /* version */
        *(uint8_t *)(buf + 1) = 0x4; /* type */
        *(uint32_t *)(buf + 8) = U32_HTON(0x1018); /* experimenter */
        *(uint32_t *)(buf + 12) = U32_HTON(0x6); /* subtype */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup ofdpa_mpls_vpn_label_remark_action_mod_msg ofdpa_mpls_vpn_label_remark_action_mod_msg
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
ofdpa_mpls_vpn_label_remark_action_mod_msg_push_wire_values(ofdpa_mpls_vpn_label_remark_action_mod_msg_t *obj)
{

    ofdpa_mpls_vpn_label_remark_action_mod_msg_push_wire_types(obj);

    /* Message obj; set length */
    of_message_t msg;

    if ((msg = OF_OBJECT_TO_MESSAGE(obj)) != NULL) {
        of_message_length_set(msg, obj->length);
    }

    return OF_ERROR_NONE;
}

/**
 * Create a new ofdpa_mpls_vpn_label_remark_action_mod_msg object
 *
 * @param version The wire version to use for the object
 * @return Pointer to the newly create object or NULL on error
 *
 * Initializes the new object with it's default fixed length associating
 * a new underlying wire buffer.
 *
 * Use new_from_message to bind an existing message to a message object,
 * or a _get function for non-message objects.
 *
 * \ingroup ofdpa_mpls_vpn_label_remark_action_mod_msg
 */

ofdpa_mpls_vpn_label_remark_action_mod_msg_t *
ofdpa_mpls_vpn_label_remark_action_mod_msg_new(of_version_t version)
{
    ofdpa_mpls_vpn_label_remark_action_mod_msg_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MOD_MSG] + of_object_extra_len[version][OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MOD_MSG];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (ofdpa_mpls_vpn_label_remark_action_mod_msg_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    ofdpa_mpls_vpn_label_remark_action_mod_msg_init(obj, version, bytes, 0);

    if (ofdpa_mpls_vpn_label_remark_action_mod_msg_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type ofdpa_mpls_vpn_label_remark_action_mod_msg.
 *
 * @param obj Pointer to the object to initialize
 * @param version The wire version to use for the object
 * @param bytes How many bytes in the object
 * @param clean_wire Boolean: If true, clear the wire object control struct
 *
 * If bytes < 0, then the default fixed length is used for the object
 *
 * This is a "coerce" function that sets up the pointers for the
 * accessors properly.
 *
 * If anything other than 0 is passed in for the buffer size, the underlying
 * wire buffer will have 'grow' called.
 */

void
ofdpa_mpls_vpn_label_remark_action_mod_msg_init(ofdpa_mpls_vpn_label_remark_action_mod_msg_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MOD_MSG] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MOD_MSG] + of_object_extra_len[version][OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MOD_MSG];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MOD_MSG;

    /* Set up the object's function pointers */

    obj->wire_type_set = ofdpa_mpls_vpn_label_remark_action_mod_msg_push_wire_types;

    obj->wire_length_get = of_object_message_wire_length_get;
    obj->wire_length_set = of_object_message_wire_length_set;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Create a new ofdpa_mpls_vpn_label_remark_action_mod_msg object and bind it to an existing message
 *
 * @param msg The message to bind the new object to
 * @return Pointer to the newly create object or NULL on error
 *
 * \ingroup ofdpa_mpls_vpn_label_remark_action_mod_msg
 */

ofdpa_mpls_vpn_label_remark_action_mod_msg_t *
ofdpa_mpls_vpn_label_remark_action_mod_msg_new_from_message(of_message_t msg)
{
    ofdpa_mpls_vpn_label_remark_action_mod_msg_t *obj = NULL;
    of_version_t version;
    int length;

    if (msg == NULL) return NULL;

    version = of_message_version_get(msg);
    if (!OF_VERSION_OKAY(version)) return NULL;

    length = of_message_length_get(msg);

    if ((obj = (ofdpa_mpls_vpn_label_remark_action_mod_msg_t *)of_object_new(-1)) == NULL) {
        return NULL;
    }

    ofdpa_mpls_vpn_label_remark_action_mod_msg_init(obj, version, 0, 0);

    if ((of_object_buffer_bind((of_object_t *)obj, OF_MESSAGE_TO_BUFFER(msg),
                               length, OF_MESSAGE_FREE_FUNCTION)) < 0) {
       FREE(obj);
       return NULL;
    }
    obj->length = length;
    obj->version = version;

    return obj;
}

/**
 * Get xid from an object of type ofdpa_mpls_vpn_label_remark_action_mod_msg.
 * @param obj Pointer to an object of type ofdpa_mpls_vpn_label_remark_action_mod_msg.
 * @param xid Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
ofdpa_mpls_vpn_label_remark_action_mod_msg_xid_get(
    ofdpa_mpls_vpn_label_remark_action_mod_msg_t *obj,
    uint32_t *xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, xid);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set xid in an object of type ofdpa_mpls_vpn_label_remark_action_mod_msg.
 * @param obj Pointer to an object of type ofdpa_mpls_vpn_label_remark_action_mod_msg.
 * @param xid The value to write into the object
 */
void
ofdpa_mpls_vpn_label_remark_action_mod_msg_xid_set(
    ofdpa_mpls_vpn_label_remark_action_mod_msg_t *obj,
    uint32_t xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, xid);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get experimenter from an object of type ofdpa_mpls_vpn_label_remark_action_mod_msg.
 * @param obj Pointer to an object of type ofdpa_mpls_vpn_label_remark_action_mod_msg.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
ofdpa_mpls_vpn_label_remark_action_mod_msg_experimenter_get(
    ofdpa_mpls_vpn_label_remark_action_mod_msg_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 8;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set experimenter in an object of type ofdpa_mpls_vpn_label_remark_action_mod_msg.
 * @param obj Pointer to an object of type ofdpa_mpls_vpn_label_remark_action_mod_msg.
 * @param experimenter The value to write into the object
 */
void
ofdpa_mpls_vpn_label_remark_action_mod_msg_experimenter_set(
    ofdpa_mpls_vpn_label_remark_action_mod_msg_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 8;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get subtype from an object of type ofdpa_mpls_vpn_label_remark_action_mod_msg.
 * @param obj Pointer to an object of type ofdpa_mpls_vpn_label_remark_action_mod_msg.
 * @param subtype Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
ofdpa_mpls_vpn_label_remark_action_mod_msg_subtype_get(
    ofdpa_mpls_vpn_label_remark_action_mod_msg_t *obj,
    uint32_t *subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 12;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set subtype in an object of type ofdpa_mpls_vpn_label_remark_action_mod_msg.
 * @param obj Pointer to an object of type ofdpa_mpls_vpn_label_remark_action_mod_msg.
 * @param subtype The value to write into the object
 */
void
ofdpa_mpls_vpn_label_remark_action_mod_msg_subtype_set(
    ofdpa_mpls_vpn_label_remark_action_mod_msg_t *obj,
    uint32_t subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 12;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get mod_command from an object of type ofdpa_mpls_vpn_label_remark_action_mod_msg.
 * @param obj Pointer to an object of type ofdpa_mpls_vpn_label_remark_action_mod_msg.
 * @param mod_command Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
ofdpa_mpls_vpn_label_remark_action_mod_msg_mod_command_get(
    ofdpa_mpls_vpn_label_remark_action_mod_msg_t *obj,
    uint32_t *mod_command)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 16;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, mod_command);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set mod_command in an object of type ofdpa_mpls_vpn_label_remark_action_mod_msg.
 * @param obj Pointer to an object of type ofdpa_mpls_vpn_label_remark_action_mod_msg.
 * @param mod_command The value to write into the object
 */
void
ofdpa_mpls_vpn_label_remark_action_mod_msg_mod_command_set(
    ofdpa_mpls_vpn_label_remark_action_mod_msg_t *obj,
    uint32_t mod_command)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 16;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, mod_command);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get index from an object of type ofdpa_mpls_vpn_label_remark_action_mod_msg.
 * @param obj Pointer to an object of type ofdpa_mpls_vpn_label_remark_action_mod_msg.
 * @param index Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
ofdpa_mpls_vpn_label_remark_action_mod_msg_index_get(
    ofdpa_mpls_vpn_label_remark_action_mod_msg_t *obj,
    uint32_t *index)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 20;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, index);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set index in an object of type ofdpa_mpls_vpn_label_remark_action_mod_msg.
 * @param obj Pointer to an object of type ofdpa_mpls_vpn_label_remark_action_mod_msg.
 * @param index The value to write into the object
 */
void
ofdpa_mpls_vpn_label_remark_action_mod_msg_index_set(
    ofdpa_mpls_vpn_label_remark_action_mod_msg_t *obj,
    uint32_t index)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 20;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, index);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get traffic_class from an object of type ofdpa_mpls_vpn_label_remark_action_mod_msg.
 * @param obj Pointer to an object of type ofdpa_mpls_vpn_label_remark_action_mod_msg.
 * @param traffic_class Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
ofdpa_mpls_vpn_label_remark_action_mod_msg_traffic_class_get(
    ofdpa_mpls_vpn_label_remark_action_mod_msg_t *obj,
    uint8_t *traffic_class)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 24;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_get(wbuf, abs_offset, traffic_class);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set traffic_class in an object of type ofdpa_mpls_vpn_label_remark_action_mod_msg.
 * @param obj Pointer to an object of type ofdpa_mpls_vpn_label_remark_action_mod_msg.
 * @param traffic_class The value to write into the object
 */
void
ofdpa_mpls_vpn_label_remark_action_mod_msg_traffic_class_set(
    ofdpa_mpls_vpn_label_remark_action_mod_msg_t *obj,
    uint8_t traffic_class)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 24;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_set(wbuf, abs_offset, traffic_class);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get color from an object of type ofdpa_mpls_vpn_label_remark_action_mod_msg.
 * @param obj Pointer to an object of type ofdpa_mpls_vpn_label_remark_action_mod_msg.
 * @param color Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
ofdpa_mpls_vpn_label_remark_action_mod_msg_color_get(
    ofdpa_mpls_vpn_label_remark_action_mod_msg_t *obj,
    uint8_t *color)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 25;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_get(wbuf, abs_offset, color);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set color in an object of type ofdpa_mpls_vpn_label_remark_action_mod_msg.
 * @param obj Pointer to an object of type ofdpa_mpls_vpn_label_remark_action_mod_msg.
 * @param color The value to write into the object
 */
void
ofdpa_mpls_vpn_label_remark_action_mod_msg_color_set(
    ofdpa_mpls_vpn_label_remark_action_mod_msg_t *obj,
    uint8_t color)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 25;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_set(wbuf, abs_offset, color);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get mpls_tc from an object of type ofdpa_mpls_vpn_label_remark_action_mod_msg.
 * @param obj Pointer to an object of type ofdpa_mpls_vpn_label_remark_action_mod_msg.
 * @param mpls_tc Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
ofdpa_mpls_vpn_label_remark_action_mod_msg_mpls_tc_get(
    ofdpa_mpls_vpn_label_remark_action_mod_msg_t *obj,
    uint8_t *mpls_tc)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 26;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_get(wbuf, abs_offset, mpls_tc);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set mpls_tc in an object of type ofdpa_mpls_vpn_label_remark_action_mod_msg.
 * @param obj Pointer to an object of type ofdpa_mpls_vpn_label_remark_action_mod_msg.
 * @param mpls_tc The value to write into the object
 */
void
ofdpa_mpls_vpn_label_remark_action_mod_msg_mpls_tc_set(
    ofdpa_mpls_vpn_label_remark_action_mod_msg_t *obj,
    uint8_t mpls_tc)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 26;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_set(wbuf, abs_offset, mpls_tc);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get vlan_pcp from an object of type ofdpa_mpls_vpn_label_remark_action_mod_msg.
 * @param obj Pointer to an object of type ofdpa_mpls_vpn_label_remark_action_mod_msg.
 * @param vlan_pcp Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
ofdpa_mpls_vpn_label_remark_action_mod_msg_vlan_pcp_get(
    ofdpa_mpls_vpn_label_remark_action_mod_msg_t *obj,
    uint8_t *vlan_pcp)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 27;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_get(wbuf, abs_offset, vlan_pcp);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set vlan_pcp in an object of type ofdpa_mpls_vpn_label_remark_action_mod_msg.
 * @param obj Pointer to an object of type ofdpa_mpls_vpn_label_remark_action_mod_msg.
 * @param vlan_pcp The value to write into the object
 */
void
ofdpa_mpls_vpn_label_remark_action_mod_msg_vlan_pcp_set(
    ofdpa_mpls_vpn_label_remark_action_mod_msg_t *obj,
    uint8_t vlan_pcp)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 27;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_set(wbuf, abs_offset, vlan_pcp);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get vlan_dei from an object of type ofdpa_mpls_vpn_label_remark_action_mod_msg.
 * @param obj Pointer to an object of type ofdpa_mpls_vpn_label_remark_action_mod_msg.
 * @param vlan_dei Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
ofdpa_mpls_vpn_label_remark_action_mod_msg_vlan_dei_get(
    ofdpa_mpls_vpn_label_remark_action_mod_msg_t *obj,
    uint8_t *vlan_dei)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 28;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_get(wbuf, abs_offset, vlan_dei);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set vlan_dei in an object of type ofdpa_mpls_vpn_label_remark_action_mod_msg.
 * @param obj Pointer to an object of type ofdpa_mpls_vpn_label_remark_action_mod_msg.
 * @param vlan_dei The value to write into the object
 */
void
ofdpa_mpls_vpn_label_remark_action_mod_msg_vlan_dei_set(
    ofdpa_mpls_vpn_label_remark_action_mod_msg_t *obj,
    uint8_t vlan_dei)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 28;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_set(wbuf, abs_offset, vlan_dei);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}
/* Copyright (c) 2008 The Board of Trustees of The Leland Stanford Junior University */
/* Copyright (c) 2011, 2012 Open Networking Foundation */
/* Copyright (c) 2012, 2013 Big Switch Networks, Inc. */
/* See the file LICENSE.loci which should have been included in the source distribution */
#ifdef __GNUC__

#ifdef __linux__
/* glibc */
#include <features.h>
#else
/* NetBSD etc */
#include <sys/cdefs.h>
#ifdef __GNUC_PREREQ__
#define __GNUC_PREREQ __GNUC_PREREQ__
#endif
#endif

#ifndef __GNUC_PREREQ
/* fallback */
#define __GNUC_PREREQ(maj, min) 0
#endif

#if __GNUC_PREREQ(4,6)
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif

#endif

#include "loci_log.h"
#include "loci_int.h"

static void
ofdpa_mpls_vpn_label_remark_action_multipart_reply_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint8_t *)(buf + 0) = obj->version; /* version */
        *(uint8_t *)(buf + 1) = 0x13; /* type */
        *(uint16_t *)(buf + 8) = U16_HTON(0xffff); /* stats_type */
        *(uint32_t *)(buf + 16) = U32_HTON(0x1018); /* experimenter */
        *(uint32_t *)(buf + 20) = U32_HTON(0x7); /* subtype */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup ofdpa_mpls_vpn_label_remark_action_multipart_reply ofdpa_mpls_vpn_label_remark_action_multipart_reply
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
ofdpa_mpls_vpn_label_remark_action_multipart_reply_push_wire_values(ofdpa_mpls_vpn_label_remark_action_multipart_reply_t *obj)
{

    ofdpa_mpls_vpn_label_remark_action_multipart_reply_push_wire_types(obj);

    /* Message obj; set length */
    of_message_t msg;

    if ((msg = OF_OBJECT_TO_MESSAGE(obj)) != NULL) {
        of_message_length_set(msg, obj->length);
    }

    return OF_ERROR_NONE;
}

/**
 * Create a new ofdpa_mpls_vpn_label_remark_action_multipart_reply object
 *
 * @param version The wire version to use for the object
 * @return Pointer to the newly create object or NULL on error
 *
 * Initializes the new object with it's default fixed length associating
 * a new underlying wire buffer.
 *
 * Use new_from_message to bind an existing message to a message object,
 * or a _get function for non-message objects.
 *
 * \ingroup ofdpa_mpls_vpn_label_remark_action_multipart_reply
 */

ofdpa_mpls_vpn_label_remark_action_multipart_reply_t *
ofdpa_mpls_vpn_label_remark_action_multipart_reply_new(of_version_t version)
{
    ofdpa_mpls_vpn_label_remark_action_multipart_reply_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MULTIPART_REPLY] + of_object_extra_len[version][OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MULTIPART_REPLY];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (ofdpa_mpls_vpn_label_remark_action_multipart_reply_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    ofdpa_mpls_vpn_label_remark_action_multipart_reply_init(obj, version, bytes, 0);

    if (ofdpa_mpls_vpn_label_remark_action_multipart_reply_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type ofdpa_mpls_vpn_label_remark_action_multipart_reply.
 *
 * @param obj Pointer to the object to initialize
 * @param version The wire version to use for the object
 * @param bytes How many bytes in the object
 * @param clean_wire Boolean: If true, clear the wire object control struct
 *
 * If bytes < 0, then the default fixed length is used for the object
 *
 * This is a "coerce" function that sets up the pointers for the
 * accessors properly.
 *
 * If anything other than 0 is passed in for the buffer size, the underlying
 * wire buffer will have 'grow' called.
 */

void
ofdpa_mpls_vpn_label_remark_action_multipart_reply_init(ofdpa_mpls_vpn_label_remark_action_multipart_reply_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MULTIPART_REPLY] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MULTIPART_REPLY] + of_object_extra_len[version][OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MULTIPART_REPLY];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MULTIPART_REPLY;

    /* Set up the object's function pointers */

    obj->wire_type_set = ofdpa_mpls_vpn_label_remark_action_multipart_reply_push_wire_types;

    obj->wire_length_get = of_object_message_wire_length_get;
    obj->wire_length_set = of_object_message_wire_length_set;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Create a new ofdpa_mpls_vpn_label_remark_action_multipart_reply object and bind it to an existing message
 *
 * @param msg The message to bind the new object to
 * @return Pointer to the newly create object or NULL on error
 *
 * \ingroup ofdpa_mpls_vpn_label_remark_action_multipart_reply
 */

ofdpa_mpls_vpn_label_remark_action_multipart_reply_t *
ofdpa_mpls_vpn_label_remark_action_multipart_reply_new_from_message(of_message_t msg)
{
    ofdpa_mpls_vpn_label_remark_action_multipart_reply_t *obj = NULL;
    of_version_t version;
    int length;

    if (msg == NULL) return NULL;

    version = of_message_version_get(msg);
    if (!OF_VERSION_OKAY(version)) return NULL;

    length = of_message_length_get(msg);

    if ((obj = (ofdpa_mpls_vpn_label_remark_action_multipart_reply_t *)of_object_new(-1)) == NULL) {
        return NULL;
    }

    ofdpa_mpls_vpn_label_remark_action_multipart_reply_init(obj, version, 0, 0);

    if ((of_object_buffer_bind((of_object_t *)obj, OF_MESSAGE_TO_BUFFER(msg),
                               length, OF_MESSAGE_FREE_FUNCTION)) < 0) {
       FREE(obj);
       return NULL;
    }
    obj->length = length;
    obj->version = version;

    return obj;
}

/**
 * Get xid from an object of type ofdpa_mpls_vpn_label_remark_action_multipart_reply.
 * @param obj Pointer to an object of type ofdpa_mpls_vpn_label_remark_action_multipart_reply.
 * @param xid Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
ofdpa_mpls_vpn_label_remark_action_multipart_reply_xid_get(
    ofdpa_mpls_vpn_label_remark_action_multipart_reply_t *obj,
    uint32_t *xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, xid);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set xid in an object of type ofdpa_mpls_vpn_label_remark_action_multipart_reply.
 * @param obj Pointer to an object of type ofdpa_mpls_vpn_label_remark_action_multipart_reply.
 * @param xid The value to write into the object
 */
void
ofdpa_mpls_vpn_label_remark_action_multipart_reply_xid_set(
    ofdpa_mpls_vpn_label_remark_action_multipart_reply_t *obj,
    uint32_t xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, xid);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get flags from an object of type ofdpa_mpls_vpn_label_remark_action_multipart_reply.
 * @param obj Pointer to an object of type ofdpa_mpls_vpn_label_remark_action_multipart_reply.
 * @param flags Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
ofdpa_mpls_vpn_label_remark_action_multipart_reply_flags_get(
    ofdpa_mpls_vpn_label_remark_action_multipart_reply_t *obj,
    uint16_t *flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 10;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_get(wbuf, abs_offset, flags);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set flags in an object of type ofdpa_mpls_vpn_label_remark_action_multipart_reply.
 * @param obj Pointer to an object of type ofdpa_mpls_vpn_label_remark_action_multipart_reply.
 * @param flags The value to write into the object
 */
void
ofdpa_mpls_vpn_label_remark_action_multipart_reply_flags_set(
    ofdpa_mpls_vpn_label_remark_action_multipart_reply_t *obj,
    uint16_t flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 10;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_set(wbuf, abs_offset, flags);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get experimenter from an object of type ofdpa_mpls_vpn_label_remark_action_multipart_reply.
 * @param obj Pointer to an object of type ofdpa_mpls_vpn_label_remark_action_multipart_reply.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
ofdpa_mpls_vpn_label_remark_action_multipart_reply_experimenter_get(
    ofdpa_mpls_vpn_label_remark_action_multipart_reply_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 16;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set experimenter in an object of type ofdpa_mpls_vpn_label_remark_action_multipart_reply.
 * @param obj Pointer to an object of type ofdpa_mpls_vpn_label_remark_action_multipart_reply.
 * @param experimenter The value to write into the object
 */
void
ofdpa_mpls_vpn_label_remark_action_multipart_reply_experimenter_set(
    ofdpa_mpls_vpn_label_remark_action_multipart_reply_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 16;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get subtype from an object of type ofdpa_mpls_vpn_label_remark_action_multipart_reply.
 * @param obj Pointer to an object of type ofdpa_mpls_vpn_label_remark_action_multipart_reply.
 * @param subtype Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
ofdpa_mpls_vpn_label_remark_action_multipart_reply_subtype_get(
    ofdpa_mpls_vpn_label_remark_action_multipart_reply_t *obj,
    uint32_t *subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 20;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set subtype in an object of type ofdpa_mpls_vpn_label_remark_action_multipart_reply.
 * @param obj Pointer to an object of type ofdpa_mpls_vpn_label_remark_action_multipart_reply.
 * @param subtype The value to write into the object
 */
void
ofdpa_mpls_vpn_label_remark_action_multipart_reply_subtype_set(
    ofdpa_mpls_vpn_label_remark_action_multipart_reply_t *obj,
    uint32_t subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 20;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get index from an object of type ofdpa_mpls_vpn_label_remark_action_multipart_reply.
 * @param obj Pointer to an object of type ofdpa_mpls_vpn_label_remark_action_multipart_reply.
 * @param index Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
ofdpa_mpls_vpn_label_remark_action_multipart_reply_index_get(
    ofdpa_mpls_vpn_label_remark_action_multipart_reply_t *obj,
    uint32_t *index)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 24;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, index);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set index in an object of type ofdpa_mpls_vpn_label_remark_action_multipart_reply.
 * @param obj Pointer to an object of type ofdpa_mpls_vpn_label_remark_action_multipart_reply.
 * @param index The value to write into the object
 */
void
ofdpa_mpls_vpn_label_remark_action_multipart_reply_index_set(
    ofdpa_mpls_vpn_label_remark_action_multipart_reply_t *obj,
    uint32_t index)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 24;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, index);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get traffic_class from an object of type ofdpa_mpls_vpn_label_remark_action_multipart_reply.
 * @param obj Pointer to an object of type ofdpa_mpls_vpn_label_remark_action_multipart_reply.
 * @param traffic_class Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
ofdpa_mpls_vpn_label_remark_action_multipart_reply_traffic_class_get(
    ofdpa_mpls_vpn_label_remark_action_multipart_reply_t *obj,
    uint8_t *traffic_class)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 28;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_get(wbuf, abs_offset, traffic_class);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set traffic_class in an object of type ofdpa_mpls_vpn_label_remark_action_multipart_reply.
 * @param obj Pointer to an object of type ofdpa_mpls_vpn_label_remark_action_multipart_reply.
 * @param traffic_class The value to write into the object
 */
void
ofdpa_mpls_vpn_label_remark_action_multipart_reply_traffic_class_set(
    ofdpa_mpls_vpn_label_remark_action_multipart_reply_t *obj,
    uint8_t traffic_class)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 28;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_set(wbuf, abs_offset, traffic_class);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get color from an object of type ofdpa_mpls_vpn_label_remark_action_multipart_reply.
 * @param obj Pointer to an object of type ofdpa_mpls_vpn_label_remark_action_multipart_reply.
 * @param color Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
ofdpa_mpls_vpn_label_remark_action_multipart_reply_color_get(
    ofdpa_mpls_vpn_label_remark_action_multipart_reply_t *obj,
    uint8_t *color)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 29;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_get(wbuf, abs_offset, color);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set color in an object of type ofdpa_mpls_vpn_label_remark_action_multipart_reply.
 * @param obj Pointer to an object of type ofdpa_mpls_vpn_label_remark_action_multipart_reply.
 * @param color The value to write into the object
 */
void
ofdpa_mpls_vpn_label_remark_action_multipart_reply_color_set(
    ofdpa_mpls_vpn_label_remark_action_multipart_reply_t *obj,
    uint8_t color)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 29;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_set(wbuf, abs_offset, color);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get mpls_tc from an object of type ofdpa_mpls_vpn_label_remark_action_multipart_reply.
 * @param obj Pointer to an object of type ofdpa_mpls_vpn_label_remark_action_multipart_reply.
 * @param mpls_tc Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
ofdpa_mpls_vpn_label_remark_action_multipart_reply_mpls_tc_get(
    ofdpa_mpls_vpn_label_remark_action_multipart_reply_t *obj,
    uint8_t *mpls_tc)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 30;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_get(wbuf, abs_offset, mpls_tc);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set mpls_tc in an object of type ofdpa_mpls_vpn_label_remark_action_multipart_reply.
 * @param obj Pointer to an object of type ofdpa_mpls_vpn_label_remark_action_multipart_reply.
 * @param mpls_tc The value to write into the object
 */
void
ofdpa_mpls_vpn_label_remark_action_multipart_reply_mpls_tc_set(
    ofdpa_mpls_vpn_label_remark_action_multipart_reply_t *obj,
    uint8_t mpls_tc)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 30;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_set(wbuf, abs_offset, mpls_tc);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get vlan_pcp from an object of type ofdpa_mpls_vpn_label_remark_action_multipart_reply.
 * @param obj Pointer to an object of type ofdpa_mpls_vpn_label_remark_action_multipart_reply.
 * @param vlan_pcp Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
ofdpa_mpls_vpn_label_remark_action_multipart_reply_vlan_pcp_get(
    ofdpa_mpls_vpn_label_remark_action_multipart_reply_t *obj,
    uint8_t *vlan_pcp)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 31;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_get(wbuf, abs_offset, vlan_pcp);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set vlan_pcp in an object of type ofdpa_mpls_vpn_label_remark_action_multipart_reply.
 * @param obj Pointer to an object of type ofdpa_mpls_vpn_label_remark_action_multipart_reply.
 * @param vlan_pcp The value to write into the object
 */
void
ofdpa_mpls_vpn_label_remark_action_multipart_reply_vlan_pcp_set(
    ofdpa_mpls_vpn_label_remark_action_multipart_reply_t *obj,
    uint8_t vlan_pcp)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 31;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_set(wbuf, abs_offset, vlan_pcp);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get vlan_dei from an object of type ofdpa_mpls_vpn_label_remark_action_multipart_reply.
 * @param obj Pointer to an object of type ofdpa_mpls_vpn_label_remark_action_multipart_reply.
 * @param vlan_dei Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
ofdpa_mpls_vpn_label_remark_action_multipart_reply_vlan_dei_get(
    ofdpa_mpls_vpn_label_remark_action_multipart_reply_t *obj,
    uint8_t *vlan_dei)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 32;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_get(wbuf, abs_offset, vlan_dei);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set vlan_dei in an object of type ofdpa_mpls_vpn_label_remark_action_multipart_reply.
 * @param obj Pointer to an object of type ofdpa_mpls_vpn_label_remark_action_multipart_reply.
 * @param vlan_dei The value to write into the object
 */
void
ofdpa_mpls_vpn_label_remark_action_multipart_reply_vlan_dei_set(
    ofdpa_mpls_vpn_label_remark_action_multipart_reply_t *obj,
    uint8_t vlan_dei)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 32;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_set(wbuf, abs_offset, vlan_dei);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}
/* Copyright (c) 2008 The Board of Trustees of The Leland Stanford Junior University */
/* Copyright (c) 2011, 2012 Open Networking Foundation */
/* Copyright (c) 2012, 2013 Big Switch Networks, Inc. */
/* See the file LICENSE.loci which should have been included in the source distribution */
#ifdef __GNUC__

#ifdef __linux__
/* glibc */
#include <features.h>
#else
/* NetBSD etc */
#include <sys/cdefs.h>
#ifdef __GNUC_PREREQ__
#define __GNUC_PREREQ __GNUC_PREREQ__
#endif
#endif

#ifndef __GNUC_PREREQ
/* fallback */
#define __GNUC_PREREQ(maj, min) 0
#endif

#if __GNUC_PREREQ(4,6)
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif

#endif

#include "loci_log.h"
#include "loci_int.h"

static void
ofdpa_mpls_vpn_label_remark_action_multipart_request_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint8_t *)(buf + 0) = obj->version; /* version */
        *(uint8_t *)(buf + 1) = 0x12; /* type */
        *(uint16_t *)(buf + 8) = U16_HTON(0xffff); /* stats_type */
        *(uint32_t *)(buf + 16) = U32_HTON(0x1018); /* experimenter */
        *(uint32_t *)(buf + 20) = U32_HTON(0x7); /* subtype */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup ofdpa_mpls_vpn_label_remark_action_multipart_request ofdpa_mpls_vpn_label_remark_action_multipart_request
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
ofdpa_mpls_vpn_label_remark_action_multipart_request_push_wire_values(ofdpa_mpls_vpn_label_remark_action_multipart_request_t *obj)
{

    ofdpa_mpls_vpn_label_remark_action_multipart_request_push_wire_types(obj);

    /* Message obj; set length */
    of_message_t msg;

    if ((msg = OF_OBJECT_TO_MESSAGE(obj)) != NULL) {
        of_message_length_set(msg, obj->length);
    }

    return OF_ERROR_NONE;
}

/**
 * Create a new ofdpa_mpls_vpn_label_remark_action_multipart_request object
 *
 * @param version The wire version to use for the object
 * @return Pointer to the newly create object or NULL on error
 *
 * Initializes the new object with it's default fixed length associating
 * a new underlying wire buffer.
 *
 * Use new_from_message to bind an existing message to a message object,
 * or a _get function for non-message objects.
 *
 * \ingroup ofdpa_mpls_vpn_label_remark_action_multipart_request
 */

ofdpa_mpls_vpn_label_remark_action_multipart_request_t *
ofdpa_mpls_vpn_label_remark_action_multipart_request_new(of_version_t version)
{
    ofdpa_mpls_vpn_label_remark_action_multipart_request_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MULTIPART_REQUEST] + of_object_extra_len[version][OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MULTIPART_REQUEST];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (ofdpa_mpls_vpn_label_remark_action_multipart_request_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    ofdpa_mpls_vpn_label_remark_action_multipart_request_init(obj, version, bytes, 0);

    if (ofdpa_mpls_vpn_label_remark_action_multipart_request_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type ofdpa_mpls_vpn_label_remark_action_multipart_request.
 *
 * @param obj Pointer to the object to initialize
 * @param version The wire version to use for the object
 * @param bytes How many bytes in the object
 * @param clean_wire Boolean: If true, clear the wire object control struct
 *
 * If bytes < 0, then the default fixed length is used for the object
 *
 * This is a "coerce" function that sets up the pointers for the
 * accessors properly.
 *
 * If anything other than 0 is passed in for the buffer size, the underlying
 * wire buffer will have 'grow' called.
 */

void
ofdpa_mpls_vpn_label_remark_action_multipart_request_init(ofdpa_mpls_vpn_label_remark_action_multipart_request_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MULTIPART_REQUEST] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MULTIPART_REQUEST] + of_object_extra_len[version][OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MULTIPART_REQUEST];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MULTIPART_REQUEST;

    /* Set up the object's function pointers */

    obj->wire_type_set = ofdpa_mpls_vpn_label_remark_action_multipart_request_push_wire_types;

    obj->wire_length_get = of_object_message_wire_length_get;
    obj->wire_length_set = of_object_message_wire_length_set;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Create a new ofdpa_mpls_vpn_label_remark_action_multipart_request object and bind it to an existing message
 *
 * @param msg The message to bind the new object to
 * @return Pointer to the newly create object or NULL on error
 *
 * \ingroup ofdpa_mpls_vpn_label_remark_action_multipart_request
 */

ofdpa_mpls_vpn_label_remark_action_multipart_request_t *
ofdpa_mpls_vpn_label_remark_action_multipart_request_new_from_message(of_message_t msg)
{
    ofdpa_mpls_vpn_label_remark_action_multipart_request_t *obj = NULL;
    of_version_t version;
    int length;

    if (msg == NULL) return NULL;

    version = of_message_version_get(msg);
    if (!OF_VERSION_OKAY(version)) return NULL;

    length = of_message_length_get(msg);

    if ((obj = (ofdpa_mpls_vpn_label_remark_action_multipart_request_t *)of_object_new(-1)) == NULL) {
        return NULL;
    }

    ofdpa_mpls_vpn_label_remark_action_multipart_request_init(obj, version, 0, 0);

    if ((of_object_buffer_bind((of_object_t *)obj, OF_MESSAGE_TO_BUFFER(msg),
                               length, OF_MESSAGE_FREE_FUNCTION)) < 0) {
       FREE(obj);
       return NULL;
    }
    obj->length = length;
    obj->version = version;

    return obj;
}

/**
 * Get xid from an object of type ofdpa_mpls_vpn_label_remark_action_multipart_request.
 * @param obj Pointer to an object of type ofdpa_mpls_vpn_label_remark_action_multipart_request.
 * @param xid Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
ofdpa_mpls_vpn_label_remark_action_multipart_request_xid_get(
    ofdpa_mpls_vpn_label_remark_action_multipart_request_t *obj,
    uint32_t *xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, xid);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set xid in an object of type ofdpa_mpls_vpn_label_remark_action_multipart_request.
 * @param obj Pointer to an object of type ofdpa_mpls_vpn_label_remark_action_multipart_request.
 * @param xid The value to write into the object
 */
void
ofdpa_mpls_vpn_label_remark_action_multipart_request_xid_set(
    ofdpa_mpls_vpn_label_remark_action_multipart_request_t *obj,
    uint32_t xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, xid);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get flags from an object of type ofdpa_mpls_vpn_label_remark_action_multipart_request.
 * @param obj Pointer to an object of type ofdpa_mpls_vpn_label_remark_action_multipart_request.
 * @param flags Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
ofdpa_mpls_vpn_label_remark_action_multipart_request_flags_get(
    ofdpa_mpls_vpn_label_remark_action_multipart_request_t *obj,
    uint16_t *flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 10;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_get(wbuf, abs_offset, flags);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set flags in an object of type ofdpa_mpls_vpn_label_remark_action_multipart_request.
 * @param obj Pointer to an object of type ofdpa_mpls_vpn_label_remark_action_multipart_request.
 * @param flags The value to write into the object
 */
void
ofdpa_mpls_vpn_label_remark_action_multipart_request_flags_set(
    ofdpa_mpls_vpn_label_remark_action_multipart_request_t *obj,
    uint16_t flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 10;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_set(wbuf, abs_offset, flags);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get experimenter from an object of type ofdpa_mpls_vpn_label_remark_action_multipart_request.
 * @param obj Pointer to an object of type ofdpa_mpls_vpn_label_remark_action_multipart_request.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
ofdpa_mpls_vpn_label_remark_action_multipart_request_experimenter_get(
    ofdpa_mpls_vpn_label_remark_action_multipart_request_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 16;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set experimenter in an object of type ofdpa_mpls_vpn_label_remark_action_multipart_request.
 * @param obj Pointer to an object of type ofdpa_mpls_vpn_label_remark_action_multipart_request.
 * @param experimenter The value to write into the object
 */
void
ofdpa_mpls_vpn_label_remark_action_multipart_request_experimenter_set(
    ofdpa_mpls_vpn_label_remark_action_multipart_request_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 16;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get subtype from an object of type ofdpa_mpls_vpn_label_remark_action_multipart_request.
 * @param obj Pointer to an object of type ofdpa_mpls_vpn_label_remark_action_multipart_request.
 * @param subtype Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
ofdpa_mpls_vpn_label_remark_action_multipart_request_subtype_get(
    ofdpa_mpls_vpn_label_remark_action_multipart_request_t *obj,
    uint32_t *subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 20;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set subtype in an object of type ofdpa_mpls_vpn_label_remark_action_multipart_request.
 * @param obj Pointer to an object of type ofdpa_mpls_vpn_label_remark_action_multipart_request.
 * @param subtype The value to write into the object
 */
void
ofdpa_mpls_vpn_label_remark_action_multipart_request_subtype_set(
    ofdpa_mpls_vpn_label_remark_action_multipart_request_t *obj,
    uint32_t subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 20;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get index from an object of type ofdpa_mpls_vpn_label_remark_action_multipart_request.
 * @param obj Pointer to an object of type ofdpa_mpls_vpn_label_remark_action_multipart_request.
 * @param index Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
ofdpa_mpls_vpn_label_remark_action_multipart_request_index_get(
    ofdpa_mpls_vpn_label_remark_action_multipart_request_t *obj,
    uint32_t *index)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 24;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, index);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set index in an object of type ofdpa_mpls_vpn_label_remark_action_multipart_request.
 * @param obj Pointer to an object of type ofdpa_mpls_vpn_label_remark_action_multipart_request.
 * @param index The value to write into the object
 */
void
ofdpa_mpls_vpn_label_remark_action_multipart_request_index_set(
    ofdpa_mpls_vpn_label_remark_action_multipart_request_t *obj,
    uint32_t index)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 24;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, index);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get traffic_class from an object of type ofdpa_mpls_vpn_label_remark_action_multipart_request.
 * @param obj Pointer to an object of type ofdpa_mpls_vpn_label_remark_action_multipart_request.
 * @param traffic_class Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
ofdpa_mpls_vpn_label_remark_action_multipart_request_traffic_class_get(
    ofdpa_mpls_vpn_label_remark_action_multipart_request_t *obj,
    uint8_t *traffic_class)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 28;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_get(wbuf, abs_offset, traffic_class);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set traffic_class in an object of type ofdpa_mpls_vpn_label_remark_action_multipart_request.
 * @param obj Pointer to an object of type ofdpa_mpls_vpn_label_remark_action_multipart_request.
 * @param traffic_class The value to write into the object
 */
void
ofdpa_mpls_vpn_label_remark_action_multipart_request_traffic_class_set(
    ofdpa_mpls_vpn_label_remark_action_multipart_request_t *obj,
    uint8_t traffic_class)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 28;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_set(wbuf, abs_offset, traffic_class);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get color from an object of type ofdpa_mpls_vpn_label_remark_action_multipart_request.
 * @param obj Pointer to an object of type ofdpa_mpls_vpn_label_remark_action_multipart_request.
 * @param color Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
ofdpa_mpls_vpn_label_remark_action_multipart_request_color_get(
    ofdpa_mpls_vpn_label_remark_action_multipart_request_t *obj,
    uint8_t *color)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 29;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_get(wbuf, abs_offset, color);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set color in an object of type ofdpa_mpls_vpn_label_remark_action_multipart_request.
 * @param obj Pointer to an object of type ofdpa_mpls_vpn_label_remark_action_multipart_request.
 * @param color The value to write into the object
 */
void
ofdpa_mpls_vpn_label_remark_action_multipart_request_color_set(
    ofdpa_mpls_vpn_label_remark_action_multipart_request_t *obj,
    uint8_t color)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 29;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_set(wbuf, abs_offset, color);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get mpls_tc from an object of type ofdpa_mpls_vpn_label_remark_action_multipart_request.
 * @param obj Pointer to an object of type ofdpa_mpls_vpn_label_remark_action_multipart_request.
 * @param mpls_tc Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
ofdpa_mpls_vpn_label_remark_action_multipart_request_mpls_tc_get(
    ofdpa_mpls_vpn_label_remark_action_multipart_request_t *obj,
    uint8_t *mpls_tc)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 30;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_get(wbuf, abs_offset, mpls_tc);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set mpls_tc in an object of type ofdpa_mpls_vpn_label_remark_action_multipart_request.
 * @param obj Pointer to an object of type ofdpa_mpls_vpn_label_remark_action_multipart_request.
 * @param mpls_tc The value to write into the object
 */
void
ofdpa_mpls_vpn_label_remark_action_multipart_request_mpls_tc_set(
    ofdpa_mpls_vpn_label_remark_action_multipart_request_t *obj,
    uint8_t mpls_tc)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 30;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_set(wbuf, abs_offset, mpls_tc);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get vlan_pcp from an object of type ofdpa_mpls_vpn_label_remark_action_multipart_request.
 * @param obj Pointer to an object of type ofdpa_mpls_vpn_label_remark_action_multipart_request.
 * @param vlan_pcp Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
ofdpa_mpls_vpn_label_remark_action_multipart_request_vlan_pcp_get(
    ofdpa_mpls_vpn_label_remark_action_multipart_request_t *obj,
    uint8_t *vlan_pcp)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 31;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_get(wbuf, abs_offset, vlan_pcp);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set vlan_pcp in an object of type ofdpa_mpls_vpn_label_remark_action_multipart_request.
 * @param obj Pointer to an object of type ofdpa_mpls_vpn_label_remark_action_multipart_request.
 * @param vlan_pcp The value to write into the object
 */
void
ofdpa_mpls_vpn_label_remark_action_multipart_request_vlan_pcp_set(
    ofdpa_mpls_vpn_label_remark_action_multipart_request_t *obj,
    uint8_t vlan_pcp)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 31;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_set(wbuf, abs_offset, vlan_pcp);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get vlan_dei from an object of type ofdpa_mpls_vpn_label_remark_action_multipart_request.
 * @param obj Pointer to an object of type ofdpa_mpls_vpn_label_remark_action_multipart_request.
 * @param vlan_dei Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
ofdpa_mpls_vpn_label_remark_action_multipart_request_vlan_dei_get(
    ofdpa_mpls_vpn_label_remark_action_multipart_request_t *obj,
    uint8_t *vlan_dei)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 32;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_get(wbuf, abs_offset, vlan_dei);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set vlan_dei in an object of type ofdpa_mpls_vpn_label_remark_action_multipart_request.
 * @param obj Pointer to an object of type ofdpa_mpls_vpn_label_remark_action_multipart_request.
 * @param vlan_dei The value to write into the object
 */
void
ofdpa_mpls_vpn_label_remark_action_multipart_request_vlan_dei_set(
    ofdpa_mpls_vpn_label_remark_action_multipart_request_t *obj,
    uint8_t vlan_dei)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 32;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_set(wbuf, abs_offset, vlan_dei);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}
/* Copyright (c) 2008 The Board of Trustees of The Leland Stanford Junior University */
/* Copyright (c) 2011, 2012 Open Networking Foundation */
/* Copyright (c) 2012, 2013 Big Switch Networks, Inc. */
/* See the file LICENSE.loci which should have been included in the source distribution */
#ifdef __GNUC__

#ifdef __linux__
/* glibc */
#include <features.h>
#else
/* NetBSD etc */
#include <sys/cdefs.h>
#ifdef __GNUC_PREREQ__
#define __GNUC_PREREQ __GNUC_PREREQ__
#endif
#endif

#ifndef __GNUC_PREREQ
/* fallback */
#define __GNUC_PREREQ(maj, min) 0
#endif

#if __GNUC_PREREQ(4,6)
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif

#endif

#include "loci_log.h"
#include "loci_int.h"

static void
ofdpa_oam_dataplane_ctr_mod_msg_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint8_t *)(buf + 0) = obj->version; /* version */
        *(uint8_t *)(buf + 1) = 0x4; /* type */
        *(uint32_t *)(buf + 8) = U32_HTON(0x1018); /* experimenter */
        *(uint32_t *)(buf + 12) = U32_HTON(0x2); /* subtype */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup ofdpa_oam_dataplane_ctr_mod_msg ofdpa_oam_dataplane_ctr_mod_msg
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
ofdpa_oam_dataplane_ctr_mod_msg_push_wire_values(ofdpa_oam_dataplane_ctr_mod_msg_t *obj)
{

    ofdpa_oam_dataplane_ctr_mod_msg_push_wire_types(obj);

    /* Message obj; set length */
    of_message_t msg;

    if ((msg = OF_OBJECT_TO_MESSAGE(obj)) != NULL) {
        of_message_length_set(msg, obj->length);
    }

    return OF_ERROR_NONE;
}

/**
 * Create a new ofdpa_oam_dataplane_ctr_mod_msg object
 *
 * @param version The wire version to use for the object
 * @return Pointer to the newly create object or NULL on error
 *
 * Initializes the new object with it's default fixed length associating
 * a new underlying wire buffer.
 *
 * Use new_from_message to bind an existing message to a message object,
 * or a _get function for non-message objects.
 *
 * \ingroup ofdpa_oam_dataplane_ctr_mod_msg
 */

ofdpa_oam_dataplane_ctr_mod_msg_t *
ofdpa_oam_dataplane_ctr_mod_msg_new(of_version_t version)
{
    ofdpa_oam_dataplane_ctr_mod_msg_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OFDPA_OAM_DATAPLANE_CTR_MOD_MSG] + of_object_extra_len[version][OFDPA_OAM_DATAPLANE_CTR_MOD_MSG];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (ofdpa_oam_dataplane_ctr_mod_msg_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    ofdpa_oam_dataplane_ctr_mod_msg_init(obj, version, bytes, 0);

    if (ofdpa_oam_dataplane_ctr_mod_msg_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type ofdpa_oam_dataplane_ctr_mod_msg.
 *
 * @param obj Pointer to the object to initialize
 * @param version The wire version to use for the object
 * @param bytes How many bytes in the object
 * @param clean_wire Boolean: If true, clear the wire object control struct
 *
 * If bytes < 0, then the default fixed length is used for the object
 *
 * This is a "coerce" function that sets up the pointers for the
 * accessors properly.
 *
 * If anything other than 0 is passed in for the buffer size, the underlying
 * wire buffer will have 'grow' called.
 */

void
ofdpa_oam_dataplane_ctr_mod_msg_init(ofdpa_oam_dataplane_ctr_mod_msg_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OFDPA_OAM_DATAPLANE_CTR_MOD_MSG] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OFDPA_OAM_DATAPLANE_CTR_MOD_MSG] + of_object_extra_len[version][OFDPA_OAM_DATAPLANE_CTR_MOD_MSG];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OFDPA_OAM_DATAPLANE_CTR_MOD_MSG;

    /* Set up the object's function pointers */

    obj->wire_type_set = ofdpa_oam_dataplane_ctr_mod_msg_push_wire_types;

    obj->wire_length_get = of_object_message_wire_length_get;
    obj->wire_length_set = of_object_message_wire_length_set;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Create a new ofdpa_oam_dataplane_ctr_mod_msg object and bind it to an existing message
 *
 * @param msg The message to bind the new object to
 * @return Pointer to the newly create object or NULL on error
 *
 * \ingroup ofdpa_oam_dataplane_ctr_mod_msg
 */

ofdpa_oam_dataplane_ctr_mod_msg_t *
ofdpa_oam_dataplane_ctr_mod_msg_new_from_message(of_message_t msg)
{
    ofdpa_oam_dataplane_ctr_mod_msg_t *obj = NULL;
    of_version_t version;
    int length;

    if (msg == NULL) return NULL;

    version = of_message_version_get(msg);
    if (!OF_VERSION_OKAY(version)) return NULL;

    length = of_message_length_get(msg);

    if ((obj = (ofdpa_oam_dataplane_ctr_mod_msg_t *)of_object_new(-1)) == NULL) {
        return NULL;
    }

    ofdpa_oam_dataplane_ctr_mod_msg_init(obj, version, 0, 0);

    if ((of_object_buffer_bind((of_object_t *)obj, OF_MESSAGE_TO_BUFFER(msg),
                               length, OF_MESSAGE_FREE_FUNCTION)) < 0) {
       FREE(obj);
       return NULL;
    }
    obj->length = length;
    obj->version = version;

    return obj;
}

/**
 * Get xid from an object of type ofdpa_oam_dataplane_ctr_mod_msg.
 * @param obj Pointer to an object of type ofdpa_oam_dataplane_ctr_mod_msg.
 * @param xid Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
ofdpa_oam_dataplane_ctr_mod_msg_xid_get(
    ofdpa_oam_dataplane_ctr_mod_msg_t *obj,
    uint32_t *xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_OAM_DATAPLANE_CTR_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, xid);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set xid in an object of type ofdpa_oam_dataplane_ctr_mod_msg.
 * @param obj Pointer to an object of type ofdpa_oam_dataplane_ctr_mod_msg.
 * @param xid The value to write into the object
 */
void
ofdpa_oam_dataplane_ctr_mod_msg_xid_set(
    ofdpa_oam_dataplane_ctr_mod_msg_t *obj,
    uint32_t xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_OAM_DATAPLANE_CTR_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, xid);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get experimenter from an object of type ofdpa_oam_dataplane_ctr_mod_msg.
 * @param obj Pointer to an object of type ofdpa_oam_dataplane_ctr_mod_msg.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
ofdpa_oam_dataplane_ctr_mod_msg_experimenter_get(
    ofdpa_oam_dataplane_ctr_mod_msg_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_OAM_DATAPLANE_CTR_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 8;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set experimenter in an object of type ofdpa_oam_dataplane_ctr_mod_msg.
 * @param obj Pointer to an object of type ofdpa_oam_dataplane_ctr_mod_msg.
 * @param experimenter The value to write into the object
 */
void
ofdpa_oam_dataplane_ctr_mod_msg_experimenter_set(
    ofdpa_oam_dataplane_ctr_mod_msg_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_OAM_DATAPLANE_CTR_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 8;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get subtype from an object of type ofdpa_oam_dataplane_ctr_mod_msg.
 * @param obj Pointer to an object of type ofdpa_oam_dataplane_ctr_mod_msg.
 * @param subtype Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
ofdpa_oam_dataplane_ctr_mod_msg_subtype_get(
    ofdpa_oam_dataplane_ctr_mod_msg_t *obj,
    uint32_t *subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_OAM_DATAPLANE_CTR_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 12;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set subtype in an object of type ofdpa_oam_dataplane_ctr_mod_msg.
 * @param obj Pointer to an object of type ofdpa_oam_dataplane_ctr_mod_msg.
 * @param subtype The value to write into the object
 */
void
ofdpa_oam_dataplane_ctr_mod_msg_subtype_set(
    ofdpa_oam_dataplane_ctr_mod_msg_t *obj,
    uint32_t subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_OAM_DATAPLANE_CTR_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 12;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get mod_command from an object of type ofdpa_oam_dataplane_ctr_mod_msg.
 * @param obj Pointer to an object of type ofdpa_oam_dataplane_ctr_mod_msg.
 * @param mod_command Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
ofdpa_oam_dataplane_ctr_mod_msg_mod_command_get(
    ofdpa_oam_dataplane_ctr_mod_msg_t *obj,
    uint32_t *mod_command)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_OAM_DATAPLANE_CTR_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 16;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, mod_command);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set mod_command in an object of type ofdpa_oam_dataplane_ctr_mod_msg.
 * @param obj Pointer to an object of type ofdpa_oam_dataplane_ctr_mod_msg.
 * @param mod_command The value to write into the object
 */
void
ofdpa_oam_dataplane_ctr_mod_msg_mod_command_set(
    ofdpa_oam_dataplane_ctr_mod_msg_t *obj,
    uint32_t mod_command)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_OAM_DATAPLANE_CTR_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 16;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, mod_command);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get lmep_id from an object of type ofdpa_oam_dataplane_ctr_mod_msg.
 * @param obj Pointer to an object of type ofdpa_oam_dataplane_ctr_mod_msg.
 * @param lmep_id Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
ofdpa_oam_dataplane_ctr_mod_msg_lmep_id_get(
    ofdpa_oam_dataplane_ctr_mod_msg_t *obj,
    uint32_t *lmep_id)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_OAM_DATAPLANE_CTR_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 20;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, lmep_id);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set lmep_id in an object of type ofdpa_oam_dataplane_ctr_mod_msg.
 * @param obj Pointer to an object of type ofdpa_oam_dataplane_ctr_mod_msg.
 * @param lmep_id The value to write into the object
 */
void
ofdpa_oam_dataplane_ctr_mod_msg_lmep_id_set(
    ofdpa_oam_dataplane_ctr_mod_msg_t *obj,
    uint32_t lmep_id)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_OAM_DATAPLANE_CTR_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 20;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, lmep_id);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get transmit_packets from an object of type ofdpa_oam_dataplane_ctr_mod_msg.
 * @param obj Pointer to an object of type ofdpa_oam_dataplane_ctr_mod_msg.
 * @param transmit_packets Pointer to the child object of type
 * uint64_t to be filled out.
 *
 */
void
ofdpa_oam_dataplane_ctr_mod_msg_transmit_packets_get(
    ofdpa_oam_dataplane_ctr_mod_msg_t *obj,
    uint64_t *transmit_packets)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_OAM_DATAPLANE_CTR_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 24;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u64_get(wbuf, abs_offset, transmit_packets);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set transmit_packets in an object of type ofdpa_oam_dataplane_ctr_mod_msg.
 * @param obj Pointer to an object of type ofdpa_oam_dataplane_ctr_mod_msg.
 * @param transmit_packets The value to write into the object
 */
void
ofdpa_oam_dataplane_ctr_mod_msg_transmit_packets_set(
    ofdpa_oam_dataplane_ctr_mod_msg_t *obj,
    uint64_t transmit_packets)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_OAM_DATAPLANE_CTR_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 24;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u64_set(wbuf, abs_offset, transmit_packets);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get receive_packets from an object of type ofdpa_oam_dataplane_ctr_mod_msg.
 * @param obj Pointer to an object of type ofdpa_oam_dataplane_ctr_mod_msg.
 * @param receive_packets Pointer to the child object of type
 * uint64_t to be filled out.
 *
 */
void
ofdpa_oam_dataplane_ctr_mod_msg_receive_packets_get(
    ofdpa_oam_dataplane_ctr_mod_msg_t *obj,
    uint64_t *receive_packets)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_OAM_DATAPLANE_CTR_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 32;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u64_get(wbuf, abs_offset, receive_packets);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set receive_packets in an object of type ofdpa_oam_dataplane_ctr_mod_msg.
 * @param obj Pointer to an object of type ofdpa_oam_dataplane_ctr_mod_msg.
 * @param receive_packets The value to write into the object
 */
void
ofdpa_oam_dataplane_ctr_mod_msg_receive_packets_set(
    ofdpa_oam_dataplane_ctr_mod_msg_t *obj,
    uint64_t receive_packets)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_OAM_DATAPLANE_CTR_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 32;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u64_set(wbuf, abs_offset, receive_packets);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get traffic_class from an object of type ofdpa_oam_dataplane_ctr_mod_msg.
 * @param obj Pointer to an object of type ofdpa_oam_dataplane_ctr_mod_msg.
 * @param traffic_class Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
ofdpa_oam_dataplane_ctr_mod_msg_traffic_class_get(
    ofdpa_oam_dataplane_ctr_mod_msg_t *obj,
    uint8_t *traffic_class)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_OAM_DATAPLANE_CTR_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 40;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_get(wbuf, abs_offset, traffic_class);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set traffic_class in an object of type ofdpa_oam_dataplane_ctr_mod_msg.
 * @param obj Pointer to an object of type ofdpa_oam_dataplane_ctr_mod_msg.
 * @param traffic_class The value to write into the object
 */
void
ofdpa_oam_dataplane_ctr_mod_msg_traffic_class_set(
    ofdpa_oam_dataplane_ctr_mod_msg_t *obj,
    uint8_t traffic_class)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_OAM_DATAPLANE_CTR_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 40;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_set(wbuf, abs_offset, traffic_class);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}
/* Copyright (c) 2008 The Board of Trustees of The Leland Stanford Junior University */
/* Copyright (c) 2011, 2012 Open Networking Foundation */
/* Copyright (c) 2012, 2013 Big Switch Networks, Inc. */
/* See the file LICENSE.loci which should have been included in the source distribution */
#ifdef __GNUC__

#ifdef __linux__
/* glibc */
#include <features.h>
#else
/* NetBSD etc */
#include <sys/cdefs.h>
#ifdef __GNUC_PREREQ__
#define __GNUC_PREREQ __GNUC_PREREQ__
#endif
#endif

#ifndef __GNUC_PREREQ
/* fallback */
#define __GNUC_PREREQ(maj, min) 0
#endif

#if __GNUC_PREREQ(4,6)
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif

#endif

#include "loci_log.h"
#include "loci_int.h"

static void
ofdpa_oam_dataplane_ctr_multipart_reply_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint8_t *)(buf + 0) = obj->version; /* version */
        *(uint8_t *)(buf + 1) = 0x13; /* type */
        *(uint16_t *)(buf + 8) = U16_HTON(0xffff); /* stats_type */
        *(uint32_t *)(buf + 16) = U32_HTON(0x1018); /* experimenter */
        *(uint32_t *)(buf + 20) = U32_HTON(0x3); /* subtype */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup ofdpa_oam_dataplane_ctr_multipart_reply ofdpa_oam_dataplane_ctr_multipart_reply
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
ofdpa_oam_dataplane_ctr_multipart_reply_push_wire_values(ofdpa_oam_dataplane_ctr_multipart_reply_t *obj)
{

    ofdpa_oam_dataplane_ctr_multipart_reply_push_wire_types(obj);

    /* Message obj; set length */
    of_message_t msg;

    if ((msg = OF_OBJECT_TO_MESSAGE(obj)) != NULL) {
        of_message_length_set(msg, obj->length);
    }

    return OF_ERROR_NONE;
}

/**
 * Create a new ofdpa_oam_dataplane_ctr_multipart_reply object
 *
 * @param version The wire version to use for the object
 * @return Pointer to the newly create object or NULL on error
 *
 * Initializes the new object with it's default fixed length associating
 * a new underlying wire buffer.
 *
 * Use new_from_message to bind an existing message to a message object,
 * or a _get function for non-message objects.
 *
 * \ingroup ofdpa_oam_dataplane_ctr_multipart_reply
 */

ofdpa_oam_dataplane_ctr_multipart_reply_t *
ofdpa_oam_dataplane_ctr_multipart_reply_new(of_version_t version)
{
    ofdpa_oam_dataplane_ctr_multipart_reply_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OFDPA_OAM_DATAPLANE_CTR_MULTIPART_REPLY] + of_object_extra_len[version][OFDPA_OAM_DATAPLANE_CTR_MULTIPART_REPLY];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (ofdpa_oam_dataplane_ctr_multipart_reply_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    ofdpa_oam_dataplane_ctr_multipart_reply_init(obj, version, bytes, 0);

    if (ofdpa_oam_dataplane_ctr_multipart_reply_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type ofdpa_oam_dataplane_ctr_multipart_reply.
 *
 * @param obj Pointer to the object to initialize
 * @param version The wire version to use for the object
 * @param bytes How many bytes in the object
 * @param clean_wire Boolean: If true, clear the wire object control struct
 *
 * If bytes < 0, then the default fixed length is used for the object
 *
 * This is a "coerce" function that sets up the pointers for the
 * accessors properly.
 *
 * If anything other than 0 is passed in for the buffer size, the underlying
 * wire buffer will have 'grow' called.
 */

void
ofdpa_oam_dataplane_ctr_multipart_reply_init(ofdpa_oam_dataplane_ctr_multipart_reply_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OFDPA_OAM_DATAPLANE_CTR_MULTIPART_REPLY] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OFDPA_OAM_DATAPLANE_CTR_MULTIPART_REPLY] + of_object_extra_len[version][OFDPA_OAM_DATAPLANE_CTR_MULTIPART_REPLY];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OFDPA_OAM_DATAPLANE_CTR_MULTIPART_REPLY;

    /* Set up the object's function pointers */

    obj->wire_type_set = ofdpa_oam_dataplane_ctr_multipart_reply_push_wire_types;

    obj->wire_length_get = of_object_message_wire_length_get;
    obj->wire_length_set = of_object_message_wire_length_set;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Create a new ofdpa_oam_dataplane_ctr_multipart_reply object and bind it to an existing message
 *
 * @param msg The message to bind the new object to
 * @return Pointer to the newly create object or NULL on error
 *
 * \ingroup ofdpa_oam_dataplane_ctr_multipart_reply
 */

ofdpa_oam_dataplane_ctr_multipart_reply_t *
ofdpa_oam_dataplane_ctr_multipart_reply_new_from_message(of_message_t msg)
{
    ofdpa_oam_dataplane_ctr_multipart_reply_t *obj = NULL;
    of_version_t version;
    int length;

    if (msg == NULL) return NULL;

    version = of_message_version_get(msg);
    if (!OF_VERSION_OKAY(version)) return NULL;

    length = of_message_length_get(msg);

    if ((obj = (ofdpa_oam_dataplane_ctr_multipart_reply_t *)of_object_new(-1)) == NULL) {
        return NULL;
    }

    ofdpa_oam_dataplane_ctr_multipart_reply_init(obj, version, 0, 0);

    if ((of_object_buffer_bind((of_object_t *)obj, OF_MESSAGE_TO_BUFFER(msg),
                               length, OF_MESSAGE_FREE_FUNCTION)) < 0) {
       FREE(obj);
       return NULL;
    }
    obj->length = length;
    obj->version = version;

    return obj;
}

/**
 * Get xid from an object of type ofdpa_oam_dataplane_ctr_multipart_reply.
 * @param obj Pointer to an object of type ofdpa_oam_dataplane_ctr_multipart_reply.
 * @param xid Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
ofdpa_oam_dataplane_ctr_multipart_reply_xid_get(
    ofdpa_oam_dataplane_ctr_multipart_reply_t *obj,
    uint32_t *xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_OAM_DATAPLANE_CTR_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, xid);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set xid in an object of type ofdpa_oam_dataplane_ctr_multipart_reply.
 * @param obj Pointer to an object of type ofdpa_oam_dataplane_ctr_multipart_reply.
 * @param xid The value to write into the object
 */
void
ofdpa_oam_dataplane_ctr_multipart_reply_xid_set(
    ofdpa_oam_dataplane_ctr_multipart_reply_t *obj,
    uint32_t xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_OAM_DATAPLANE_CTR_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, xid);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get flags from an object of type ofdpa_oam_dataplane_ctr_multipart_reply.
 * @param obj Pointer to an object of type ofdpa_oam_dataplane_ctr_multipart_reply.
 * @param flags Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
ofdpa_oam_dataplane_ctr_multipart_reply_flags_get(
    ofdpa_oam_dataplane_ctr_multipart_reply_t *obj,
    uint16_t *flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_OAM_DATAPLANE_CTR_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 10;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_get(wbuf, abs_offset, flags);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set flags in an object of type ofdpa_oam_dataplane_ctr_multipart_reply.
 * @param obj Pointer to an object of type ofdpa_oam_dataplane_ctr_multipart_reply.
 * @param flags The value to write into the object
 */
void
ofdpa_oam_dataplane_ctr_multipart_reply_flags_set(
    ofdpa_oam_dataplane_ctr_multipart_reply_t *obj,
    uint16_t flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_OAM_DATAPLANE_CTR_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 10;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_set(wbuf, abs_offset, flags);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get experimenter from an object of type ofdpa_oam_dataplane_ctr_multipart_reply.
 * @param obj Pointer to an object of type ofdpa_oam_dataplane_ctr_multipart_reply.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
ofdpa_oam_dataplane_ctr_multipart_reply_experimenter_get(
    ofdpa_oam_dataplane_ctr_multipart_reply_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_OAM_DATAPLANE_CTR_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 16;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set experimenter in an object of type ofdpa_oam_dataplane_ctr_multipart_reply.
 * @param obj Pointer to an object of type ofdpa_oam_dataplane_ctr_multipart_reply.
 * @param experimenter The value to write into the object
 */
void
ofdpa_oam_dataplane_ctr_multipart_reply_experimenter_set(
    ofdpa_oam_dataplane_ctr_multipart_reply_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_OAM_DATAPLANE_CTR_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 16;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get subtype from an object of type ofdpa_oam_dataplane_ctr_multipart_reply.
 * @param obj Pointer to an object of type ofdpa_oam_dataplane_ctr_multipart_reply.
 * @param subtype Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
ofdpa_oam_dataplane_ctr_multipart_reply_subtype_get(
    ofdpa_oam_dataplane_ctr_multipart_reply_t *obj,
    uint32_t *subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_OAM_DATAPLANE_CTR_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 20;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set subtype in an object of type ofdpa_oam_dataplane_ctr_multipart_reply.
 * @param obj Pointer to an object of type ofdpa_oam_dataplane_ctr_multipart_reply.
 * @param subtype The value to write into the object
 */
void
ofdpa_oam_dataplane_ctr_multipart_reply_subtype_set(
    ofdpa_oam_dataplane_ctr_multipart_reply_t *obj,
    uint32_t subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_OAM_DATAPLANE_CTR_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 20;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get lmep_id from an object of type ofdpa_oam_dataplane_ctr_multipart_reply.
 * @param obj Pointer to an object of type ofdpa_oam_dataplane_ctr_multipart_reply.
 * @param lmep_id Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
ofdpa_oam_dataplane_ctr_multipart_reply_lmep_id_get(
    ofdpa_oam_dataplane_ctr_multipart_reply_t *obj,
    uint32_t *lmep_id)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_OAM_DATAPLANE_CTR_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 24;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, lmep_id);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set lmep_id in an object of type ofdpa_oam_dataplane_ctr_multipart_reply.
 * @param obj Pointer to an object of type ofdpa_oam_dataplane_ctr_multipart_reply.
 * @param lmep_id The value to write into the object
 */
void
ofdpa_oam_dataplane_ctr_multipart_reply_lmep_id_set(
    ofdpa_oam_dataplane_ctr_multipart_reply_t *obj,
    uint32_t lmep_id)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_OAM_DATAPLANE_CTR_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 24;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, lmep_id);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get receive_packets from an object of type ofdpa_oam_dataplane_ctr_multipart_reply.
 * @param obj Pointer to an object of type ofdpa_oam_dataplane_ctr_multipart_reply.
 * @param receive_packets Pointer to the child object of type
 * uint64_t to be filled out.
 *
 */
void
ofdpa_oam_dataplane_ctr_multipart_reply_receive_packets_get(
    ofdpa_oam_dataplane_ctr_multipart_reply_t *obj,
    uint64_t *receive_packets)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_OAM_DATAPLANE_CTR_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 28;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u64_get(wbuf, abs_offset, receive_packets);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set receive_packets in an object of type ofdpa_oam_dataplane_ctr_multipart_reply.
 * @param obj Pointer to an object of type ofdpa_oam_dataplane_ctr_multipart_reply.
 * @param receive_packets The value to write into the object
 */
void
ofdpa_oam_dataplane_ctr_multipart_reply_receive_packets_set(
    ofdpa_oam_dataplane_ctr_multipart_reply_t *obj,
    uint64_t receive_packets)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_OAM_DATAPLANE_CTR_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 28;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u64_set(wbuf, abs_offset, receive_packets);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get transmit_packets from an object of type ofdpa_oam_dataplane_ctr_multipart_reply.
 * @param obj Pointer to an object of type ofdpa_oam_dataplane_ctr_multipart_reply.
 * @param transmit_packets Pointer to the child object of type
 * uint64_t to be filled out.
 *
 */
void
ofdpa_oam_dataplane_ctr_multipart_reply_transmit_packets_get(
    ofdpa_oam_dataplane_ctr_multipart_reply_t *obj,
    uint64_t *transmit_packets)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_OAM_DATAPLANE_CTR_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 36;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u64_get(wbuf, abs_offset, transmit_packets);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set transmit_packets in an object of type ofdpa_oam_dataplane_ctr_multipart_reply.
 * @param obj Pointer to an object of type ofdpa_oam_dataplane_ctr_multipart_reply.
 * @param transmit_packets The value to write into the object
 */
void
ofdpa_oam_dataplane_ctr_multipart_reply_transmit_packets_set(
    ofdpa_oam_dataplane_ctr_multipart_reply_t *obj,
    uint64_t transmit_packets)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_OAM_DATAPLANE_CTR_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 36;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u64_set(wbuf, abs_offset, transmit_packets);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get reference_count from an object of type ofdpa_oam_dataplane_ctr_multipart_reply.
 * @param obj Pointer to an object of type ofdpa_oam_dataplane_ctr_multipart_reply.
 * @param reference_count Pointer to the child object of type
 * uint64_t to be filled out.
 *
 */
void
ofdpa_oam_dataplane_ctr_multipart_reply_reference_count_get(
    ofdpa_oam_dataplane_ctr_multipart_reply_t *obj,
    uint64_t *reference_count)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_OAM_DATAPLANE_CTR_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 44;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u64_get(wbuf, abs_offset, reference_count);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set reference_count in an object of type ofdpa_oam_dataplane_ctr_multipart_reply.
 * @param obj Pointer to an object of type ofdpa_oam_dataplane_ctr_multipart_reply.
 * @param reference_count The value to write into the object
 */
void
ofdpa_oam_dataplane_ctr_multipart_reply_reference_count_set(
    ofdpa_oam_dataplane_ctr_multipart_reply_t *obj,
    uint64_t reference_count)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_OAM_DATAPLANE_CTR_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 44;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u64_set(wbuf, abs_offset, reference_count);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get traffic_class from an object of type ofdpa_oam_dataplane_ctr_multipart_reply.
 * @param obj Pointer to an object of type ofdpa_oam_dataplane_ctr_multipart_reply.
 * @param traffic_class Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
ofdpa_oam_dataplane_ctr_multipart_reply_traffic_class_get(
    ofdpa_oam_dataplane_ctr_multipart_reply_t *obj,
    uint8_t *traffic_class)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_OAM_DATAPLANE_CTR_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 52;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_get(wbuf, abs_offset, traffic_class);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set traffic_class in an object of type ofdpa_oam_dataplane_ctr_multipart_reply.
 * @param obj Pointer to an object of type ofdpa_oam_dataplane_ctr_multipart_reply.
 * @param traffic_class The value to write into the object
 */
void
ofdpa_oam_dataplane_ctr_multipart_reply_traffic_class_set(
    ofdpa_oam_dataplane_ctr_multipart_reply_t *obj,
    uint8_t traffic_class)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_OAM_DATAPLANE_CTR_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 52;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_set(wbuf, abs_offset, traffic_class);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}
/* Copyright (c) 2008 The Board of Trustees of The Leland Stanford Junior University */
/* Copyright (c) 2011, 2012 Open Networking Foundation */
/* Copyright (c) 2012, 2013 Big Switch Networks, Inc. */
/* See the file LICENSE.loci which should have been included in the source distribution */
#ifdef __GNUC__

#ifdef __linux__
/* glibc */
#include <features.h>
#else
/* NetBSD etc */
#include <sys/cdefs.h>
#ifdef __GNUC_PREREQ__
#define __GNUC_PREREQ __GNUC_PREREQ__
#endif
#endif

#ifndef __GNUC_PREREQ
/* fallback */
#define __GNUC_PREREQ(maj, min) 0
#endif

#if __GNUC_PREREQ(4,6)
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif

#endif

#include "loci_log.h"
#include "loci_int.h"

static void
ofdpa_oam_dataplane_ctr_multipart_request_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint8_t *)(buf + 0) = obj->version; /* version */
        *(uint8_t *)(buf + 1) = 0x12; /* type */
        *(uint16_t *)(buf + 8) = U16_HTON(0xffff); /* stats_type */
        *(uint32_t *)(buf + 16) = U32_HTON(0x1018); /* experimenter */
        *(uint32_t *)(buf + 20) = U32_HTON(0x3); /* subtype */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup ofdpa_oam_dataplane_ctr_multipart_request ofdpa_oam_dataplane_ctr_multipart_request
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
ofdpa_oam_dataplane_ctr_multipart_request_push_wire_values(ofdpa_oam_dataplane_ctr_multipart_request_t *obj)
{

    ofdpa_oam_dataplane_ctr_multipart_request_push_wire_types(obj);

    /* Message obj; set length */
    of_message_t msg;

    if ((msg = OF_OBJECT_TO_MESSAGE(obj)) != NULL) {
        of_message_length_set(msg, obj->length);
    }

    return OF_ERROR_NONE;
}

/**
 * Create a new ofdpa_oam_dataplane_ctr_multipart_request object
 *
 * @param version The wire version to use for the object
 * @return Pointer to the newly create object or NULL on error
 *
 * Initializes the new object with it's default fixed length associating
 * a new underlying wire buffer.
 *
 * Use new_from_message to bind an existing message to a message object,
 * or a _get function for non-message objects.
 *
 * \ingroup ofdpa_oam_dataplane_ctr_multipart_request
 */

ofdpa_oam_dataplane_ctr_multipart_request_t *
ofdpa_oam_dataplane_ctr_multipart_request_new(of_version_t version)
{
    ofdpa_oam_dataplane_ctr_multipart_request_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OFDPA_OAM_DATAPLANE_CTR_MULTIPART_REQUEST] + of_object_extra_len[version][OFDPA_OAM_DATAPLANE_CTR_MULTIPART_REQUEST];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (ofdpa_oam_dataplane_ctr_multipart_request_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    ofdpa_oam_dataplane_ctr_multipart_request_init(obj, version, bytes, 0);

    if (ofdpa_oam_dataplane_ctr_multipart_request_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type ofdpa_oam_dataplane_ctr_multipart_request.
 *
 * @param obj Pointer to the object to initialize
 * @param version The wire version to use for the object
 * @param bytes How many bytes in the object
 * @param clean_wire Boolean: If true, clear the wire object control struct
 *
 * If bytes < 0, then the default fixed length is used for the object
 *
 * This is a "coerce" function that sets up the pointers for the
 * accessors properly.
 *
 * If anything other than 0 is passed in for the buffer size, the underlying
 * wire buffer will have 'grow' called.
 */

void
ofdpa_oam_dataplane_ctr_multipart_request_init(ofdpa_oam_dataplane_ctr_multipart_request_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OFDPA_OAM_DATAPLANE_CTR_MULTIPART_REQUEST] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OFDPA_OAM_DATAPLANE_CTR_MULTIPART_REQUEST] + of_object_extra_len[version][OFDPA_OAM_DATAPLANE_CTR_MULTIPART_REQUEST];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OFDPA_OAM_DATAPLANE_CTR_MULTIPART_REQUEST;

    /* Set up the object's function pointers */

    obj->wire_type_set = ofdpa_oam_dataplane_ctr_multipart_request_push_wire_types;

    obj->wire_length_get = of_object_message_wire_length_get;
    obj->wire_length_set = of_object_message_wire_length_set;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Create a new ofdpa_oam_dataplane_ctr_multipart_request object and bind it to an existing message
 *
 * @param msg The message to bind the new object to
 * @return Pointer to the newly create object or NULL on error
 *
 * \ingroup ofdpa_oam_dataplane_ctr_multipart_request
 */

ofdpa_oam_dataplane_ctr_multipart_request_t *
ofdpa_oam_dataplane_ctr_multipart_request_new_from_message(of_message_t msg)
{
    ofdpa_oam_dataplane_ctr_multipart_request_t *obj = NULL;
    of_version_t version;
    int length;

    if (msg == NULL) return NULL;

    version = of_message_version_get(msg);
    if (!OF_VERSION_OKAY(version)) return NULL;

    length = of_message_length_get(msg);

    if ((obj = (ofdpa_oam_dataplane_ctr_multipart_request_t *)of_object_new(-1)) == NULL) {
        return NULL;
    }

    ofdpa_oam_dataplane_ctr_multipart_request_init(obj, version, 0, 0);

    if ((of_object_buffer_bind((of_object_t *)obj, OF_MESSAGE_TO_BUFFER(msg),
                               length, OF_MESSAGE_FREE_FUNCTION)) < 0) {
       FREE(obj);
       return NULL;
    }
    obj->length = length;
    obj->version = version;

    return obj;
}

/**
 * Get xid from an object of type ofdpa_oam_dataplane_ctr_multipart_request.
 * @param obj Pointer to an object of type ofdpa_oam_dataplane_ctr_multipart_request.
 * @param xid Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
ofdpa_oam_dataplane_ctr_multipart_request_xid_get(
    ofdpa_oam_dataplane_ctr_multipart_request_t *obj,
    uint32_t *xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_OAM_DATAPLANE_CTR_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, xid);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set xid in an object of type ofdpa_oam_dataplane_ctr_multipart_request.
 * @param obj Pointer to an object of type ofdpa_oam_dataplane_ctr_multipart_request.
 * @param xid The value to write into the object
 */
void
ofdpa_oam_dataplane_ctr_multipart_request_xid_set(
    ofdpa_oam_dataplane_ctr_multipart_request_t *obj,
    uint32_t xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_OAM_DATAPLANE_CTR_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, xid);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get flags from an object of type ofdpa_oam_dataplane_ctr_multipart_request.
 * @param obj Pointer to an object of type ofdpa_oam_dataplane_ctr_multipart_request.
 * @param flags Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
ofdpa_oam_dataplane_ctr_multipart_request_flags_get(
    ofdpa_oam_dataplane_ctr_multipart_request_t *obj,
    uint16_t *flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_OAM_DATAPLANE_CTR_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 10;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_get(wbuf, abs_offset, flags);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set flags in an object of type ofdpa_oam_dataplane_ctr_multipart_request.
 * @param obj Pointer to an object of type ofdpa_oam_dataplane_ctr_multipart_request.
 * @param flags The value to write into the object
 */
void
ofdpa_oam_dataplane_ctr_multipart_request_flags_set(
    ofdpa_oam_dataplane_ctr_multipart_request_t *obj,
    uint16_t flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_OAM_DATAPLANE_CTR_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 10;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_set(wbuf, abs_offset, flags);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get experimenter from an object of type ofdpa_oam_dataplane_ctr_multipart_request.
 * @param obj Pointer to an object of type ofdpa_oam_dataplane_ctr_multipart_request.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
ofdpa_oam_dataplane_ctr_multipart_request_experimenter_get(
    ofdpa_oam_dataplane_ctr_multipart_request_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_OAM_DATAPLANE_CTR_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 16;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set experimenter in an object of type ofdpa_oam_dataplane_ctr_multipart_request.
 * @param obj Pointer to an object of type ofdpa_oam_dataplane_ctr_multipart_request.
 * @param experimenter The value to write into the object
 */
void
ofdpa_oam_dataplane_ctr_multipart_request_experimenter_set(
    ofdpa_oam_dataplane_ctr_multipart_request_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_OAM_DATAPLANE_CTR_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 16;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get subtype from an object of type ofdpa_oam_dataplane_ctr_multipart_request.
 * @param obj Pointer to an object of type ofdpa_oam_dataplane_ctr_multipart_request.
 * @param subtype Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
ofdpa_oam_dataplane_ctr_multipart_request_subtype_get(
    ofdpa_oam_dataplane_ctr_multipart_request_t *obj,
    uint32_t *subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_OAM_DATAPLANE_CTR_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 20;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set subtype in an object of type ofdpa_oam_dataplane_ctr_multipart_request.
 * @param obj Pointer to an object of type ofdpa_oam_dataplane_ctr_multipart_request.
 * @param subtype The value to write into the object
 */
void
ofdpa_oam_dataplane_ctr_multipart_request_subtype_set(
    ofdpa_oam_dataplane_ctr_multipart_request_t *obj,
    uint32_t subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_OAM_DATAPLANE_CTR_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 20;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get lmep_id from an object of type ofdpa_oam_dataplane_ctr_multipart_request.
 * @param obj Pointer to an object of type ofdpa_oam_dataplane_ctr_multipart_request.
 * @param lmep_id Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
ofdpa_oam_dataplane_ctr_multipart_request_lmep_id_get(
    ofdpa_oam_dataplane_ctr_multipart_request_t *obj,
    uint32_t *lmep_id)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_OAM_DATAPLANE_CTR_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 24;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, lmep_id);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set lmep_id in an object of type ofdpa_oam_dataplane_ctr_multipart_request.
 * @param obj Pointer to an object of type ofdpa_oam_dataplane_ctr_multipart_request.
 * @param lmep_id The value to write into the object
 */
void
ofdpa_oam_dataplane_ctr_multipart_request_lmep_id_set(
    ofdpa_oam_dataplane_ctr_multipart_request_t *obj,
    uint32_t lmep_id)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_OAM_DATAPLANE_CTR_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 24;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, lmep_id);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get transmit_packets from an object of type ofdpa_oam_dataplane_ctr_multipart_request.
 * @param obj Pointer to an object of type ofdpa_oam_dataplane_ctr_multipart_request.
 * @param transmit_packets Pointer to the child object of type
 * uint64_t to be filled out.
 *
 */
void
ofdpa_oam_dataplane_ctr_multipart_request_transmit_packets_get(
    ofdpa_oam_dataplane_ctr_multipart_request_t *obj,
    uint64_t *transmit_packets)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_OAM_DATAPLANE_CTR_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 28;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u64_get(wbuf, abs_offset, transmit_packets);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set transmit_packets in an object of type ofdpa_oam_dataplane_ctr_multipart_request.
 * @param obj Pointer to an object of type ofdpa_oam_dataplane_ctr_multipart_request.
 * @param transmit_packets The value to write into the object
 */
void
ofdpa_oam_dataplane_ctr_multipart_request_transmit_packets_set(
    ofdpa_oam_dataplane_ctr_multipart_request_t *obj,
    uint64_t transmit_packets)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_OAM_DATAPLANE_CTR_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 28;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u64_set(wbuf, abs_offset, transmit_packets);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get receive_packets from an object of type ofdpa_oam_dataplane_ctr_multipart_request.
 * @param obj Pointer to an object of type ofdpa_oam_dataplane_ctr_multipart_request.
 * @param receive_packets Pointer to the child object of type
 * uint64_t to be filled out.
 *
 */
void
ofdpa_oam_dataplane_ctr_multipart_request_receive_packets_get(
    ofdpa_oam_dataplane_ctr_multipart_request_t *obj,
    uint64_t *receive_packets)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_OAM_DATAPLANE_CTR_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 36;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u64_get(wbuf, abs_offset, receive_packets);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set receive_packets in an object of type ofdpa_oam_dataplane_ctr_multipart_request.
 * @param obj Pointer to an object of type ofdpa_oam_dataplane_ctr_multipart_request.
 * @param receive_packets The value to write into the object
 */
void
ofdpa_oam_dataplane_ctr_multipart_request_receive_packets_set(
    ofdpa_oam_dataplane_ctr_multipart_request_t *obj,
    uint64_t receive_packets)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_OAM_DATAPLANE_CTR_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 36;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u64_set(wbuf, abs_offset, receive_packets);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get reference_count from an object of type ofdpa_oam_dataplane_ctr_multipart_request.
 * @param obj Pointer to an object of type ofdpa_oam_dataplane_ctr_multipart_request.
 * @param reference_count Pointer to the child object of type
 * uint64_t to be filled out.
 *
 */
void
ofdpa_oam_dataplane_ctr_multipart_request_reference_count_get(
    ofdpa_oam_dataplane_ctr_multipart_request_t *obj,
    uint64_t *reference_count)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_OAM_DATAPLANE_CTR_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 44;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u64_get(wbuf, abs_offset, reference_count);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set reference_count in an object of type ofdpa_oam_dataplane_ctr_multipart_request.
 * @param obj Pointer to an object of type ofdpa_oam_dataplane_ctr_multipart_request.
 * @param reference_count The value to write into the object
 */
void
ofdpa_oam_dataplane_ctr_multipart_request_reference_count_set(
    ofdpa_oam_dataplane_ctr_multipart_request_t *obj,
    uint64_t reference_count)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_OAM_DATAPLANE_CTR_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 44;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u64_set(wbuf, abs_offset, reference_count);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get traffic_class from an object of type ofdpa_oam_dataplane_ctr_multipart_request.
 * @param obj Pointer to an object of type ofdpa_oam_dataplane_ctr_multipart_request.
 * @param traffic_class Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
ofdpa_oam_dataplane_ctr_multipart_request_traffic_class_get(
    ofdpa_oam_dataplane_ctr_multipart_request_t *obj,
    uint8_t *traffic_class)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_OAM_DATAPLANE_CTR_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 52;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_get(wbuf, abs_offset, traffic_class);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set traffic_class in an object of type ofdpa_oam_dataplane_ctr_multipart_request.
 * @param obj Pointer to an object of type ofdpa_oam_dataplane_ctr_multipart_request.
 * @param traffic_class The value to write into the object
 */
void
ofdpa_oam_dataplane_ctr_multipart_request_traffic_class_set(
    ofdpa_oam_dataplane_ctr_multipart_request_t *obj,
    uint8_t traffic_class)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_OAM_DATAPLANE_CTR_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 52;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_set(wbuf, abs_offset, traffic_class);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}
/* Copyright (c) 2008 The Board of Trustees of The Leland Stanford Junior University */
/* Copyright (c) 2011, 2012 Open Networking Foundation */
/* Copyright (c) 2012, 2013 Big Switch Networks, Inc. */
/* See the file LICENSE.loci which should have been included in the source distribution */
#ifdef __GNUC__

#ifdef __linux__
/* glibc */
#include <features.h>
#else
/* NetBSD etc */
#include <sys/cdefs.h>
#ifdef __GNUC_PREREQ__
#define __GNUC_PREREQ __GNUC_PREREQ__
#endif
#endif

#ifndef __GNUC_PREREQ
/* fallback */
#define __GNUC_PREREQ(maj, min) 0
#endif

#if __GNUC_PREREQ(4,6)
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif

#endif

#include "loci_log.h"
#include "loci_int.h"

static void
ofdpa_oam_drop_status_mod_msg_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint8_t *)(buf + 0) = obj->version; /* version */
        *(uint8_t *)(buf + 1) = 0x4; /* type */
        *(uint32_t *)(buf + 8) = U32_HTON(0x1018); /* experimenter */
        *(uint32_t *)(buf + 12) = U32_HTON(0x4); /* subtype */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup ofdpa_oam_drop_status_mod_msg ofdpa_oam_drop_status_mod_msg
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
ofdpa_oam_drop_status_mod_msg_push_wire_values(ofdpa_oam_drop_status_mod_msg_t *obj)
{

    ofdpa_oam_drop_status_mod_msg_push_wire_types(obj);

    /* Message obj; set length */
    of_message_t msg;

    if ((msg = OF_OBJECT_TO_MESSAGE(obj)) != NULL) {
        of_message_length_set(msg, obj->length);
    }

    return OF_ERROR_NONE;
}

/**
 * Create a new ofdpa_oam_drop_status_mod_msg object
 *
 * @param version The wire version to use for the object
 * @return Pointer to the newly create object or NULL on error
 *
 * Initializes the new object with it's default fixed length associating
 * a new underlying wire buffer.
 *
 * Use new_from_message to bind an existing message to a message object,
 * or a _get function for non-message objects.
 *
 * \ingroup ofdpa_oam_drop_status_mod_msg
 */

ofdpa_oam_drop_status_mod_msg_t *
ofdpa_oam_drop_status_mod_msg_new(of_version_t version)
{
    ofdpa_oam_drop_status_mod_msg_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OFDPA_OAM_DROP_STATUS_MOD_MSG] + of_object_extra_len[version][OFDPA_OAM_DROP_STATUS_MOD_MSG];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (ofdpa_oam_drop_status_mod_msg_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    ofdpa_oam_drop_status_mod_msg_init(obj, version, bytes, 0);

    if (ofdpa_oam_drop_status_mod_msg_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type ofdpa_oam_drop_status_mod_msg.
 *
 * @param obj Pointer to the object to initialize
 * @param version The wire version to use for the object
 * @param bytes How many bytes in the object
 * @param clean_wire Boolean: If true, clear the wire object control struct
 *
 * If bytes < 0, then the default fixed length is used for the object
 *
 * This is a "coerce" function that sets up the pointers for the
 * accessors properly.
 *
 * If anything other than 0 is passed in for the buffer size, the underlying
 * wire buffer will have 'grow' called.
 */

void
ofdpa_oam_drop_status_mod_msg_init(ofdpa_oam_drop_status_mod_msg_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OFDPA_OAM_DROP_STATUS_MOD_MSG] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OFDPA_OAM_DROP_STATUS_MOD_MSG] + of_object_extra_len[version][OFDPA_OAM_DROP_STATUS_MOD_MSG];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OFDPA_OAM_DROP_STATUS_MOD_MSG;

    /* Set up the object's function pointers */

    obj->wire_type_set = ofdpa_oam_drop_status_mod_msg_push_wire_types;

    obj->wire_length_get = of_object_message_wire_length_get;
    obj->wire_length_set = of_object_message_wire_length_set;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Create a new ofdpa_oam_drop_status_mod_msg object and bind it to an existing message
 *
 * @param msg The message to bind the new object to
 * @return Pointer to the newly create object or NULL on error
 *
 * \ingroup ofdpa_oam_drop_status_mod_msg
 */

ofdpa_oam_drop_status_mod_msg_t *
ofdpa_oam_drop_status_mod_msg_new_from_message(of_message_t msg)
{
    ofdpa_oam_drop_status_mod_msg_t *obj = NULL;
    of_version_t version;
    int length;

    if (msg == NULL) return NULL;

    version = of_message_version_get(msg);
    if (!OF_VERSION_OKAY(version)) return NULL;

    length = of_message_length_get(msg);

    if ((obj = (ofdpa_oam_drop_status_mod_msg_t *)of_object_new(-1)) == NULL) {
        return NULL;
    }

    ofdpa_oam_drop_status_mod_msg_init(obj, version, 0, 0);

    if ((of_object_buffer_bind((of_object_t *)obj, OF_MESSAGE_TO_BUFFER(msg),
                               length, OF_MESSAGE_FREE_FUNCTION)) < 0) {
       FREE(obj);
       return NULL;
    }
    obj->length = length;
    obj->version = version;

    return obj;
}

/**
 * Get xid from an object of type ofdpa_oam_drop_status_mod_msg.
 * @param obj Pointer to an object of type ofdpa_oam_drop_status_mod_msg.
 * @param xid Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
ofdpa_oam_drop_status_mod_msg_xid_get(
    ofdpa_oam_drop_status_mod_msg_t *obj,
    uint32_t *xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_OAM_DROP_STATUS_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, xid);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set xid in an object of type ofdpa_oam_drop_status_mod_msg.
 * @param obj Pointer to an object of type ofdpa_oam_drop_status_mod_msg.
 * @param xid The value to write into the object
 */
void
ofdpa_oam_drop_status_mod_msg_xid_set(
    ofdpa_oam_drop_status_mod_msg_t *obj,
    uint32_t xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_OAM_DROP_STATUS_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, xid);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get experimenter from an object of type ofdpa_oam_drop_status_mod_msg.
 * @param obj Pointer to an object of type ofdpa_oam_drop_status_mod_msg.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
ofdpa_oam_drop_status_mod_msg_experimenter_get(
    ofdpa_oam_drop_status_mod_msg_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_OAM_DROP_STATUS_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 8;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set experimenter in an object of type ofdpa_oam_drop_status_mod_msg.
 * @param obj Pointer to an object of type ofdpa_oam_drop_status_mod_msg.
 * @param experimenter The value to write into the object
 */
void
ofdpa_oam_drop_status_mod_msg_experimenter_set(
    ofdpa_oam_drop_status_mod_msg_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_OAM_DROP_STATUS_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 8;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get subtype from an object of type ofdpa_oam_drop_status_mod_msg.
 * @param obj Pointer to an object of type ofdpa_oam_drop_status_mod_msg.
 * @param subtype Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
ofdpa_oam_drop_status_mod_msg_subtype_get(
    ofdpa_oam_drop_status_mod_msg_t *obj,
    uint32_t *subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_OAM_DROP_STATUS_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 12;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set subtype in an object of type ofdpa_oam_drop_status_mod_msg.
 * @param obj Pointer to an object of type ofdpa_oam_drop_status_mod_msg.
 * @param subtype The value to write into the object
 */
void
ofdpa_oam_drop_status_mod_msg_subtype_set(
    ofdpa_oam_drop_status_mod_msg_t *obj,
    uint32_t subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_OAM_DROP_STATUS_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 12;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get mod_command from an object of type ofdpa_oam_drop_status_mod_msg.
 * @param obj Pointer to an object of type ofdpa_oam_drop_status_mod_msg.
 * @param mod_command Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
ofdpa_oam_drop_status_mod_msg_mod_command_get(
    ofdpa_oam_drop_status_mod_msg_t *obj,
    uint32_t *mod_command)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_OAM_DROP_STATUS_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 16;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, mod_command);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set mod_command in an object of type ofdpa_oam_drop_status_mod_msg.
 * @param obj Pointer to an object of type ofdpa_oam_drop_status_mod_msg.
 * @param mod_command The value to write into the object
 */
void
ofdpa_oam_drop_status_mod_msg_mod_command_set(
    ofdpa_oam_drop_status_mod_msg_t *obj,
    uint32_t mod_command)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_OAM_DROP_STATUS_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 16;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, mod_command);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get index from an object of type ofdpa_oam_drop_status_mod_msg.
 * @param obj Pointer to an object of type ofdpa_oam_drop_status_mod_msg.
 * @param index Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
ofdpa_oam_drop_status_mod_msg_index_get(
    ofdpa_oam_drop_status_mod_msg_t *obj,
    uint32_t *index)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_OAM_DROP_STATUS_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 20;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, index);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set index in an object of type ofdpa_oam_drop_status_mod_msg.
 * @param obj Pointer to an object of type ofdpa_oam_drop_status_mod_msg.
 * @param index The value to write into the object
 */
void
ofdpa_oam_drop_status_mod_msg_index_set(
    ofdpa_oam_drop_status_mod_msg_t *obj,
    uint32_t index)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_OAM_DROP_STATUS_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 20;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, index);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get entry_type from an object of type ofdpa_oam_drop_status_mod_msg.
 * @param obj Pointer to an object of type ofdpa_oam_drop_status_mod_msg.
 * @param entry_type Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
ofdpa_oam_drop_status_mod_msg_entry_type_get(
    ofdpa_oam_drop_status_mod_msg_t *obj,
    uint8_t *entry_type)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_OAM_DROP_STATUS_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 24;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_get(wbuf, abs_offset, entry_type);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set entry_type in an object of type ofdpa_oam_drop_status_mod_msg.
 * @param obj Pointer to an object of type ofdpa_oam_drop_status_mod_msg.
 * @param entry_type The value to write into the object
 */
void
ofdpa_oam_drop_status_mod_msg_entry_type_set(
    ofdpa_oam_drop_status_mod_msg_t *obj,
    uint8_t entry_type)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_OAM_DROP_STATUS_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 24;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_set(wbuf, abs_offset, entry_type);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get drop_status from an object of type ofdpa_oam_drop_status_mod_msg.
 * @param obj Pointer to an object of type ofdpa_oam_drop_status_mod_msg.
 * @param drop_status Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
ofdpa_oam_drop_status_mod_msg_drop_status_get(
    ofdpa_oam_drop_status_mod_msg_t *obj,
    uint8_t *drop_status)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_OAM_DROP_STATUS_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 25;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_get(wbuf, abs_offset, drop_status);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set drop_status in an object of type ofdpa_oam_drop_status_mod_msg.
 * @param obj Pointer to an object of type ofdpa_oam_drop_status_mod_msg.
 * @param drop_status The value to write into the object
 */
void
ofdpa_oam_drop_status_mod_msg_drop_status_set(
    ofdpa_oam_drop_status_mod_msg_t *obj,
    uint8_t drop_status)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_OAM_DROP_STATUS_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 25;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_set(wbuf, abs_offset, drop_status);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}
/* Copyright (c) 2008 The Board of Trustees of The Leland Stanford Junior University */
/* Copyright (c) 2011, 2012 Open Networking Foundation */
/* Copyright (c) 2012, 2013 Big Switch Networks, Inc. */
/* See the file LICENSE.loci which should have been included in the source distribution */
#ifdef __GNUC__

#ifdef __linux__
/* glibc */
#include <features.h>
#else
/* NetBSD etc */
#include <sys/cdefs.h>
#ifdef __GNUC_PREREQ__
#define __GNUC_PREREQ __GNUC_PREREQ__
#endif
#endif

#ifndef __GNUC_PREREQ
/* fallback */
#define __GNUC_PREREQ(maj, min) 0
#endif

#if __GNUC_PREREQ(4,6)
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif

#endif

#include "loci_log.h"
#include "loci_int.h"

static void
ofdpa_oam_drop_status_multipart_reply_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint8_t *)(buf + 0) = obj->version; /* version */
        *(uint8_t *)(buf + 1) = 0x13; /* type */
        *(uint16_t *)(buf + 8) = U16_HTON(0xffff); /* stats_type */
        *(uint32_t *)(buf + 16) = U32_HTON(0x1018); /* experimenter */
        *(uint32_t *)(buf + 20) = U32_HTON(0x5); /* subtype */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup ofdpa_oam_drop_status_multipart_reply ofdpa_oam_drop_status_multipart_reply
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
ofdpa_oam_drop_status_multipart_reply_push_wire_values(ofdpa_oam_drop_status_multipart_reply_t *obj)
{

    ofdpa_oam_drop_status_multipart_reply_push_wire_types(obj);

    /* Message obj; set length */
    of_message_t msg;

    if ((msg = OF_OBJECT_TO_MESSAGE(obj)) != NULL) {
        of_message_length_set(msg, obj->length);
    }

    return OF_ERROR_NONE;
}

/**
 * Create a new ofdpa_oam_drop_status_multipart_reply object
 *
 * @param version The wire version to use for the object
 * @return Pointer to the newly create object or NULL on error
 *
 * Initializes the new object with it's default fixed length associating
 * a new underlying wire buffer.
 *
 * Use new_from_message to bind an existing message to a message object,
 * or a _get function for non-message objects.
 *
 * \ingroup ofdpa_oam_drop_status_multipart_reply
 */

ofdpa_oam_drop_status_multipart_reply_t *
ofdpa_oam_drop_status_multipart_reply_new(of_version_t version)
{
    ofdpa_oam_drop_status_multipart_reply_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OFDPA_OAM_DROP_STATUS_MULTIPART_REPLY] + of_object_extra_len[version][OFDPA_OAM_DROP_STATUS_MULTIPART_REPLY];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (ofdpa_oam_drop_status_multipart_reply_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    ofdpa_oam_drop_status_multipart_reply_init(obj, version, bytes, 0);

    if (ofdpa_oam_drop_status_multipart_reply_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type ofdpa_oam_drop_status_multipart_reply.
 *
 * @param obj Pointer to the object to initialize
 * @param version The wire version to use for the object
 * @param bytes How many bytes in the object
 * @param clean_wire Boolean: If true, clear the wire object control struct
 *
 * If bytes < 0, then the default fixed length is used for the object
 *
 * This is a "coerce" function that sets up the pointers for the
 * accessors properly.
 *
 * If anything other than 0 is passed in for the buffer size, the underlying
 * wire buffer will have 'grow' called.
 */

void
ofdpa_oam_drop_status_multipart_reply_init(ofdpa_oam_drop_status_multipart_reply_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OFDPA_OAM_DROP_STATUS_MULTIPART_REPLY] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OFDPA_OAM_DROP_STATUS_MULTIPART_REPLY] + of_object_extra_len[version][OFDPA_OAM_DROP_STATUS_MULTIPART_REPLY];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OFDPA_OAM_DROP_STATUS_MULTIPART_REPLY;

    /* Set up the object's function pointers */

    obj->wire_type_set = ofdpa_oam_drop_status_multipart_reply_push_wire_types;

    obj->wire_length_get = of_object_message_wire_length_get;
    obj->wire_length_set = of_object_message_wire_length_set;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Create a new ofdpa_oam_drop_status_multipart_reply object and bind it to an existing message
 *
 * @param msg The message to bind the new object to
 * @return Pointer to the newly create object or NULL on error
 *
 * \ingroup ofdpa_oam_drop_status_multipart_reply
 */

ofdpa_oam_drop_status_multipart_reply_t *
ofdpa_oam_drop_status_multipart_reply_new_from_message(of_message_t msg)
{
    ofdpa_oam_drop_status_multipart_reply_t *obj = NULL;
    of_version_t version;
    int length;

    if (msg == NULL) return NULL;

    version = of_message_version_get(msg);
    if (!OF_VERSION_OKAY(version)) return NULL;

    length = of_message_length_get(msg);

    if ((obj = (ofdpa_oam_drop_status_multipart_reply_t *)of_object_new(-1)) == NULL) {
        return NULL;
    }

    ofdpa_oam_drop_status_multipart_reply_init(obj, version, 0, 0);

    if ((of_object_buffer_bind((of_object_t *)obj, OF_MESSAGE_TO_BUFFER(msg),
                               length, OF_MESSAGE_FREE_FUNCTION)) < 0) {
       FREE(obj);
       return NULL;
    }
    obj->length = length;
    obj->version = version;

    return obj;
}

/**
 * Get xid from an object of type ofdpa_oam_drop_status_multipart_reply.
 * @param obj Pointer to an object of type ofdpa_oam_drop_status_multipart_reply.
 * @param xid Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
ofdpa_oam_drop_status_multipart_reply_xid_get(
    ofdpa_oam_drop_status_multipart_reply_t *obj,
    uint32_t *xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_OAM_DROP_STATUS_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, xid);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set xid in an object of type ofdpa_oam_drop_status_multipart_reply.
 * @param obj Pointer to an object of type ofdpa_oam_drop_status_multipart_reply.
 * @param xid The value to write into the object
 */
void
ofdpa_oam_drop_status_multipart_reply_xid_set(
    ofdpa_oam_drop_status_multipart_reply_t *obj,
    uint32_t xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_OAM_DROP_STATUS_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, xid);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get flags from an object of type ofdpa_oam_drop_status_multipart_reply.
 * @param obj Pointer to an object of type ofdpa_oam_drop_status_multipart_reply.
 * @param flags Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
ofdpa_oam_drop_status_multipart_reply_flags_get(
    ofdpa_oam_drop_status_multipart_reply_t *obj,
    uint16_t *flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_OAM_DROP_STATUS_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 10;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_get(wbuf, abs_offset, flags);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set flags in an object of type ofdpa_oam_drop_status_multipart_reply.
 * @param obj Pointer to an object of type ofdpa_oam_drop_status_multipart_reply.
 * @param flags The value to write into the object
 */
void
ofdpa_oam_drop_status_multipart_reply_flags_set(
    ofdpa_oam_drop_status_multipart_reply_t *obj,
    uint16_t flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_OAM_DROP_STATUS_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 10;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_set(wbuf, abs_offset, flags);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get experimenter from an object of type ofdpa_oam_drop_status_multipart_reply.
 * @param obj Pointer to an object of type ofdpa_oam_drop_status_multipart_reply.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
ofdpa_oam_drop_status_multipart_reply_experimenter_get(
    ofdpa_oam_drop_status_multipart_reply_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_OAM_DROP_STATUS_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 16;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set experimenter in an object of type ofdpa_oam_drop_status_multipart_reply.
 * @param obj Pointer to an object of type ofdpa_oam_drop_status_multipart_reply.
 * @param experimenter The value to write into the object
 */
void
ofdpa_oam_drop_status_multipart_reply_experimenter_set(
    ofdpa_oam_drop_status_multipart_reply_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_OAM_DROP_STATUS_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 16;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get subtype from an object of type ofdpa_oam_drop_status_multipart_reply.
 * @param obj Pointer to an object of type ofdpa_oam_drop_status_multipart_reply.
 * @param subtype Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
ofdpa_oam_drop_status_multipart_reply_subtype_get(
    ofdpa_oam_drop_status_multipart_reply_t *obj,
    uint32_t *subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_OAM_DROP_STATUS_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 20;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set subtype in an object of type ofdpa_oam_drop_status_multipart_reply.
 * @param obj Pointer to an object of type ofdpa_oam_drop_status_multipart_reply.
 * @param subtype The value to write into the object
 */
void
ofdpa_oam_drop_status_multipart_reply_subtype_set(
    ofdpa_oam_drop_status_multipart_reply_t *obj,
    uint32_t subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_OAM_DROP_STATUS_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 20;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get index from an object of type ofdpa_oam_drop_status_multipart_reply.
 * @param obj Pointer to an object of type ofdpa_oam_drop_status_multipart_reply.
 * @param index Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
ofdpa_oam_drop_status_multipart_reply_index_get(
    ofdpa_oam_drop_status_multipart_reply_t *obj,
    uint32_t *index)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_OAM_DROP_STATUS_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 24;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, index);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set index in an object of type ofdpa_oam_drop_status_multipart_reply.
 * @param obj Pointer to an object of type ofdpa_oam_drop_status_multipart_reply.
 * @param index The value to write into the object
 */
void
ofdpa_oam_drop_status_multipart_reply_index_set(
    ofdpa_oam_drop_status_multipart_reply_t *obj,
    uint32_t index)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_OAM_DROP_STATUS_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 24;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, index);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get entry_type from an object of type ofdpa_oam_drop_status_multipart_reply.
 * @param obj Pointer to an object of type ofdpa_oam_drop_status_multipart_reply.
 * @param entry_type Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
ofdpa_oam_drop_status_multipart_reply_entry_type_get(
    ofdpa_oam_drop_status_multipart_reply_t *obj,
    uint8_t *entry_type)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_OAM_DROP_STATUS_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 28;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_get(wbuf, abs_offset, entry_type);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set entry_type in an object of type ofdpa_oam_drop_status_multipart_reply.
 * @param obj Pointer to an object of type ofdpa_oam_drop_status_multipart_reply.
 * @param entry_type The value to write into the object
 */
void
ofdpa_oam_drop_status_multipart_reply_entry_type_set(
    ofdpa_oam_drop_status_multipart_reply_t *obj,
    uint8_t entry_type)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_OAM_DROP_STATUS_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 28;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_set(wbuf, abs_offset, entry_type);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get drop_status from an object of type ofdpa_oam_drop_status_multipart_reply.
 * @param obj Pointer to an object of type ofdpa_oam_drop_status_multipart_reply.
 * @param drop_status Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
ofdpa_oam_drop_status_multipart_reply_drop_status_get(
    ofdpa_oam_drop_status_multipart_reply_t *obj,
    uint8_t *drop_status)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_OAM_DROP_STATUS_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 29;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_get(wbuf, abs_offset, drop_status);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set drop_status in an object of type ofdpa_oam_drop_status_multipart_reply.
 * @param obj Pointer to an object of type ofdpa_oam_drop_status_multipart_reply.
 * @param drop_status The value to write into the object
 */
void
ofdpa_oam_drop_status_multipart_reply_drop_status_set(
    ofdpa_oam_drop_status_multipart_reply_t *obj,
    uint8_t drop_status)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_OAM_DROP_STATUS_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 29;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_set(wbuf, abs_offset, drop_status);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}
/* Copyright (c) 2008 The Board of Trustees of The Leland Stanford Junior University */
/* Copyright (c) 2011, 2012 Open Networking Foundation */
/* Copyright (c) 2012, 2013 Big Switch Networks, Inc. */
/* See the file LICENSE.loci which should have been included in the source distribution */
#ifdef __GNUC__

#ifdef __linux__
/* glibc */
#include <features.h>
#else
/* NetBSD etc */
#include <sys/cdefs.h>
#ifdef __GNUC_PREREQ__
#define __GNUC_PREREQ __GNUC_PREREQ__
#endif
#endif

#ifndef __GNUC_PREREQ
/* fallback */
#define __GNUC_PREREQ(maj, min) 0
#endif

#if __GNUC_PREREQ(4,6)
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif

#endif

#include "loci_log.h"
#include "loci_int.h"

static void
ofdpa_oam_drop_status_multipart_request_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint8_t *)(buf + 0) = obj->version; /* version */
        *(uint8_t *)(buf + 1) = 0x12; /* type */
        *(uint16_t *)(buf + 8) = U16_HTON(0xffff); /* stats_type */
        *(uint32_t *)(buf + 16) = U32_HTON(0x1018); /* experimenter */
        *(uint32_t *)(buf + 20) = U32_HTON(0x5); /* subtype */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup ofdpa_oam_drop_status_multipart_request ofdpa_oam_drop_status_multipart_request
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
ofdpa_oam_drop_status_multipart_request_push_wire_values(ofdpa_oam_drop_status_multipart_request_t *obj)
{

    ofdpa_oam_drop_status_multipart_request_push_wire_types(obj);

    /* Message obj; set length */
    of_message_t msg;

    if ((msg = OF_OBJECT_TO_MESSAGE(obj)) != NULL) {
        of_message_length_set(msg, obj->length);
    }

    return OF_ERROR_NONE;
}

/**
 * Create a new ofdpa_oam_drop_status_multipart_request object
 *
 * @param version The wire version to use for the object
 * @return Pointer to the newly create object or NULL on error
 *
 * Initializes the new object with it's default fixed length associating
 * a new underlying wire buffer.
 *
 * Use new_from_message to bind an existing message to a message object,
 * or a _get function for non-message objects.
 *
 * \ingroup ofdpa_oam_drop_status_multipart_request
 */

ofdpa_oam_drop_status_multipart_request_t *
ofdpa_oam_drop_status_multipart_request_new(of_version_t version)
{
    ofdpa_oam_drop_status_multipart_request_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OFDPA_OAM_DROP_STATUS_MULTIPART_REQUEST] + of_object_extra_len[version][OFDPA_OAM_DROP_STATUS_MULTIPART_REQUEST];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (ofdpa_oam_drop_status_multipart_request_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    ofdpa_oam_drop_status_multipart_request_init(obj, version, bytes, 0);

    if (ofdpa_oam_drop_status_multipart_request_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type ofdpa_oam_drop_status_multipart_request.
 *
 * @param obj Pointer to the object to initialize
 * @param version The wire version to use for the object
 * @param bytes How many bytes in the object
 * @param clean_wire Boolean: If true, clear the wire object control struct
 *
 * If bytes < 0, then the default fixed length is used for the object
 *
 * This is a "coerce" function that sets up the pointers for the
 * accessors properly.
 *
 * If anything other than 0 is passed in for the buffer size, the underlying
 * wire buffer will have 'grow' called.
 */

void
ofdpa_oam_drop_status_multipart_request_init(ofdpa_oam_drop_status_multipart_request_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OFDPA_OAM_DROP_STATUS_MULTIPART_REQUEST] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OFDPA_OAM_DROP_STATUS_MULTIPART_REQUEST] + of_object_extra_len[version][OFDPA_OAM_DROP_STATUS_MULTIPART_REQUEST];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OFDPA_OAM_DROP_STATUS_MULTIPART_REQUEST;

    /* Set up the object's function pointers */

    obj->wire_type_set = ofdpa_oam_drop_status_multipart_request_push_wire_types;

    obj->wire_length_get = of_object_message_wire_length_get;
    obj->wire_length_set = of_object_message_wire_length_set;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Create a new ofdpa_oam_drop_status_multipart_request object and bind it to an existing message
 *
 * @param msg The message to bind the new object to
 * @return Pointer to the newly create object or NULL on error
 *
 * \ingroup ofdpa_oam_drop_status_multipart_request
 */

ofdpa_oam_drop_status_multipart_request_t *
ofdpa_oam_drop_status_multipart_request_new_from_message(of_message_t msg)
{
    ofdpa_oam_drop_status_multipart_request_t *obj = NULL;
    of_version_t version;
    int length;

    if (msg == NULL) return NULL;

    version = of_message_version_get(msg);
    if (!OF_VERSION_OKAY(version)) return NULL;

    length = of_message_length_get(msg);

    if ((obj = (ofdpa_oam_drop_status_multipart_request_t *)of_object_new(-1)) == NULL) {
        return NULL;
    }

    ofdpa_oam_drop_status_multipart_request_init(obj, version, 0, 0);

    if ((of_object_buffer_bind((of_object_t *)obj, OF_MESSAGE_TO_BUFFER(msg),
                               length, OF_MESSAGE_FREE_FUNCTION)) < 0) {
       FREE(obj);
       return NULL;
    }
    obj->length = length;
    obj->version = version;

    return obj;
}

/**
 * Get xid from an object of type ofdpa_oam_drop_status_multipart_request.
 * @param obj Pointer to an object of type ofdpa_oam_drop_status_multipart_request.
 * @param xid Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
ofdpa_oam_drop_status_multipart_request_xid_get(
    ofdpa_oam_drop_status_multipart_request_t *obj,
    uint32_t *xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_OAM_DROP_STATUS_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, xid);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set xid in an object of type ofdpa_oam_drop_status_multipart_request.
 * @param obj Pointer to an object of type ofdpa_oam_drop_status_multipart_request.
 * @param xid The value to write into the object
 */
void
ofdpa_oam_drop_status_multipart_request_xid_set(
    ofdpa_oam_drop_status_multipart_request_t *obj,
    uint32_t xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_OAM_DROP_STATUS_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, xid);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get flags from an object of type ofdpa_oam_drop_status_multipart_request.
 * @param obj Pointer to an object of type ofdpa_oam_drop_status_multipart_request.
 * @param flags Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
ofdpa_oam_drop_status_multipart_request_flags_get(
    ofdpa_oam_drop_status_multipart_request_t *obj,
    uint16_t *flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_OAM_DROP_STATUS_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 10;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_get(wbuf, abs_offset, flags);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set flags in an object of type ofdpa_oam_drop_status_multipart_request.
 * @param obj Pointer to an object of type ofdpa_oam_drop_status_multipart_request.
 * @param flags The value to write into the object
 */
void
ofdpa_oam_drop_status_multipart_request_flags_set(
    ofdpa_oam_drop_status_multipart_request_t *obj,
    uint16_t flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_OAM_DROP_STATUS_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 10;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_set(wbuf, abs_offset, flags);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get experimenter from an object of type ofdpa_oam_drop_status_multipart_request.
 * @param obj Pointer to an object of type ofdpa_oam_drop_status_multipart_request.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
ofdpa_oam_drop_status_multipart_request_experimenter_get(
    ofdpa_oam_drop_status_multipart_request_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_OAM_DROP_STATUS_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 16;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set experimenter in an object of type ofdpa_oam_drop_status_multipart_request.
 * @param obj Pointer to an object of type ofdpa_oam_drop_status_multipart_request.
 * @param experimenter The value to write into the object
 */
void
ofdpa_oam_drop_status_multipart_request_experimenter_set(
    ofdpa_oam_drop_status_multipart_request_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_OAM_DROP_STATUS_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 16;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get subtype from an object of type ofdpa_oam_drop_status_multipart_request.
 * @param obj Pointer to an object of type ofdpa_oam_drop_status_multipart_request.
 * @param subtype Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
ofdpa_oam_drop_status_multipart_request_subtype_get(
    ofdpa_oam_drop_status_multipart_request_t *obj,
    uint32_t *subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_OAM_DROP_STATUS_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 20;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set subtype in an object of type ofdpa_oam_drop_status_multipart_request.
 * @param obj Pointer to an object of type ofdpa_oam_drop_status_multipart_request.
 * @param subtype The value to write into the object
 */
void
ofdpa_oam_drop_status_multipart_request_subtype_set(
    ofdpa_oam_drop_status_multipart_request_t *obj,
    uint32_t subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_OAM_DROP_STATUS_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 20;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get mod_command from an object of type ofdpa_oam_drop_status_multipart_request.
 * @param obj Pointer to an object of type ofdpa_oam_drop_status_multipart_request.
 * @param mod_command Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
ofdpa_oam_drop_status_multipart_request_mod_command_get(
    ofdpa_oam_drop_status_multipart_request_t *obj,
    uint32_t *mod_command)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_OAM_DROP_STATUS_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 24;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, mod_command);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set mod_command in an object of type ofdpa_oam_drop_status_multipart_request.
 * @param obj Pointer to an object of type ofdpa_oam_drop_status_multipart_request.
 * @param mod_command The value to write into the object
 */
void
ofdpa_oam_drop_status_multipart_request_mod_command_set(
    ofdpa_oam_drop_status_multipart_request_t *obj,
    uint32_t mod_command)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_OAM_DROP_STATUS_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 24;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, mod_command);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get index from an object of type ofdpa_oam_drop_status_multipart_request.
 * @param obj Pointer to an object of type ofdpa_oam_drop_status_multipart_request.
 * @param index Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
ofdpa_oam_drop_status_multipart_request_index_get(
    ofdpa_oam_drop_status_multipart_request_t *obj,
    uint32_t *index)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_OAM_DROP_STATUS_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 28;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, index);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set index in an object of type ofdpa_oam_drop_status_multipart_request.
 * @param obj Pointer to an object of type ofdpa_oam_drop_status_multipart_request.
 * @param index The value to write into the object
 */
void
ofdpa_oam_drop_status_multipart_request_index_set(
    ofdpa_oam_drop_status_multipart_request_t *obj,
    uint32_t index)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_OAM_DROP_STATUS_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 28;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, index);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get entry_type from an object of type ofdpa_oam_drop_status_multipart_request.
 * @param obj Pointer to an object of type ofdpa_oam_drop_status_multipart_request.
 * @param entry_type Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
ofdpa_oam_drop_status_multipart_request_entry_type_get(
    ofdpa_oam_drop_status_multipart_request_t *obj,
    uint8_t *entry_type)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_OAM_DROP_STATUS_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 32;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_get(wbuf, abs_offset, entry_type);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set entry_type in an object of type ofdpa_oam_drop_status_multipart_request.
 * @param obj Pointer to an object of type ofdpa_oam_drop_status_multipart_request.
 * @param entry_type The value to write into the object
 */
void
ofdpa_oam_drop_status_multipart_request_entry_type_set(
    ofdpa_oam_drop_status_multipart_request_t *obj,
    uint8_t entry_type)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_OAM_DROP_STATUS_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 32;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_set(wbuf, abs_offset, entry_type);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get drop_status from an object of type ofdpa_oam_drop_status_multipart_request.
 * @param obj Pointer to an object of type ofdpa_oam_drop_status_multipart_request.
 * @param drop_status Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
ofdpa_oam_drop_status_multipart_request_drop_status_get(
    ofdpa_oam_drop_status_multipart_request_t *obj,
    uint8_t *drop_status)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_OAM_DROP_STATUS_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 33;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_get(wbuf, abs_offset, drop_status);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set drop_status in an object of type ofdpa_oam_drop_status_multipart_request.
 * @param obj Pointer to an object of type ofdpa_oam_drop_status_multipart_request.
 * @param drop_status The value to write into the object
 */
void
ofdpa_oam_drop_status_multipart_request_drop_status_set(
    ofdpa_oam_drop_status_multipart_request_t *obj,
    uint8_t drop_status)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_OAM_DROP_STATUS_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 33;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_set(wbuf, abs_offset, drop_status);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}
/* Copyright (c) 2008 The Board of Trustees of The Leland Stanford Junior University */
/* Copyright (c) 2011, 2012 Open Networking Foundation */
/* Copyright (c) 2012, 2013 Big Switch Networks, Inc. */
/* See the file LICENSE.loci which should have been included in the source distribution */
#ifdef __GNUC__

#ifdef __linux__
/* glibc */
#include <features.h>
#else
/* NetBSD etc */
#include <sys/cdefs.h>
#ifdef __GNUC_PREREQ__
#define __GNUC_PREREQ __GNUC_PREREQ__
#endif
#endif

#ifndef __GNUC_PREREQ
/* fallback */
#define __GNUC_PREREQ(maj, min) 0
#endif

#if __GNUC_PREREQ(4,6)
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif

#endif

#include "loci_log.h"
#include "loci_int.h"

static void
ofdpa_port_experimenter_prop_scheduling_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint8_t *)(buf + 0) = obj->version; /* version */
        *(uint8_t *)(buf + 1) = 0x4; /* type */
        *(uint16_t *)(buf + 32) = U16_HTON(0xffff); /* pm_type */
        *(uint32_t *)(buf + 36) = U32_HTON(0x1018); /* experimenter */
        *(uint32_t *)(buf + 40) = U32_HTON(0x10); /* subtype */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup ofdpa_port_experimenter_prop_scheduling ofdpa_port_experimenter_prop_scheduling
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
ofdpa_port_experimenter_prop_scheduling_push_wire_values(ofdpa_port_experimenter_prop_scheduling_t *obj)
{

    ofdpa_port_experimenter_prop_scheduling_push_wire_types(obj);

    /* Message obj; set length */
    of_message_t msg;

    if ((msg = OF_OBJECT_TO_MESSAGE(obj)) != NULL) {
        of_message_length_set(msg, obj->length);
    }

    return OF_ERROR_NONE;
}

/**
 * Create a new ofdpa_port_experimenter_prop_scheduling object
 *
 * @param version The wire version to use for the object
 * @return Pointer to the newly create object or NULL on error
 *
 * Initializes the new object with it's default fixed length associating
 * a new underlying wire buffer.
 *
 * Use new_from_message to bind an existing message to a message object,
 * or a _get function for non-message objects.
 *
 * \ingroup ofdpa_port_experimenter_prop_scheduling
 */

ofdpa_port_experimenter_prop_scheduling_t *
ofdpa_port_experimenter_prop_scheduling_new(of_version_t version)
{
    ofdpa_port_experimenter_prop_scheduling_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OFDPA_PORT_EXPERIMENTER_PROP_SCHEDULING] + of_object_extra_len[version][OFDPA_PORT_EXPERIMENTER_PROP_SCHEDULING];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (ofdpa_port_experimenter_prop_scheduling_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    ofdpa_port_experimenter_prop_scheduling_init(obj, version, bytes, 0);

    if (ofdpa_port_experimenter_prop_scheduling_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type ofdpa_port_experimenter_prop_scheduling.
 *
 * @param obj Pointer to the object to initialize
 * @param version The wire version to use for the object
 * @param bytes How many bytes in the object
 * @param clean_wire Boolean: If true, clear the wire object control struct
 *
 * If bytes < 0, then the default fixed length is used for the object
 *
 * This is a "coerce" function that sets up the pointers for the
 * accessors properly.
 *
 * If anything other than 0 is passed in for the buffer size, the underlying
 * wire buffer will have 'grow' called.
 */

void
ofdpa_port_experimenter_prop_scheduling_init(ofdpa_port_experimenter_prop_scheduling_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OFDPA_PORT_EXPERIMENTER_PROP_SCHEDULING] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OFDPA_PORT_EXPERIMENTER_PROP_SCHEDULING] + of_object_extra_len[version][OFDPA_PORT_EXPERIMENTER_PROP_SCHEDULING];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OFDPA_PORT_EXPERIMENTER_PROP_SCHEDULING;

    /* Set up the object's function pointers */

    obj->wire_type_set = ofdpa_port_experimenter_prop_scheduling_push_wire_types;

    obj->wire_length_get = of_object_message_wire_length_get;
    obj->wire_length_set = of_object_message_wire_length_set;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Create a new ofdpa_port_experimenter_prop_scheduling object and bind it to an existing message
 *
 * @param msg The message to bind the new object to
 * @return Pointer to the newly create object or NULL on error
 *
 * \ingroup ofdpa_port_experimenter_prop_scheduling
 */

ofdpa_port_experimenter_prop_scheduling_t *
ofdpa_port_experimenter_prop_scheduling_new_from_message(of_message_t msg)
{
    ofdpa_port_experimenter_prop_scheduling_t *obj = NULL;
    of_version_t version;
    int length;

    if (msg == NULL) return NULL;

    version = of_message_version_get(msg);
    if (!OF_VERSION_OKAY(version)) return NULL;

    length = of_message_length_get(msg);

    if ((obj = (ofdpa_port_experimenter_prop_scheduling_t *)of_object_new(-1)) == NULL) {
        return NULL;
    }

    ofdpa_port_experimenter_prop_scheduling_init(obj, version, 0, 0);

    if ((of_object_buffer_bind((of_object_t *)obj, OF_MESSAGE_TO_BUFFER(msg),
                               length, OF_MESSAGE_FREE_FUNCTION)) < 0) {
       FREE(obj);
       return NULL;
    }
    obj->length = length;
    obj->version = version;

    return obj;
}

/**
 * Get xid from an object of type ofdpa_port_experimenter_prop_scheduling.
 * @param obj Pointer to an object of type ofdpa_port_experimenter_prop_scheduling.
 * @param xid Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
ofdpa_port_experimenter_prop_scheduling_xid_get(
    ofdpa_port_experimenter_prop_scheduling_t *obj,
    uint32_t *xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_PORT_EXPERIMENTER_PROP_SCHEDULING);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, xid);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set xid in an object of type ofdpa_port_experimenter_prop_scheduling.
 * @param obj Pointer to an object of type ofdpa_port_experimenter_prop_scheduling.
 * @param xid The value to write into the object
 */
void
ofdpa_port_experimenter_prop_scheduling_xid_set(
    ofdpa_port_experimenter_prop_scheduling_t *obj,
    uint32_t xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_PORT_EXPERIMENTER_PROP_SCHEDULING);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, xid);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get port_no from an object of type ofdpa_port_experimenter_prop_scheduling.
 * @param obj Pointer to an object of type ofdpa_port_experimenter_prop_scheduling.
 * @param port_no Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
ofdpa_port_experimenter_prop_scheduling_port_no_get(
    ofdpa_port_experimenter_prop_scheduling_t *obj,
    uint32_t *port_no)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_PORT_EXPERIMENTER_PROP_SCHEDULING);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 8;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, port_no);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set port_no in an object of type ofdpa_port_experimenter_prop_scheduling.
 * @param obj Pointer to an object of type ofdpa_port_experimenter_prop_scheduling.
 * @param port_no The value to write into the object
 */
void
ofdpa_port_experimenter_prop_scheduling_port_no_set(
    ofdpa_port_experimenter_prop_scheduling_t *obj,
    uint32_t port_no)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_PORT_EXPERIMENTER_PROP_SCHEDULING);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 8;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, port_no);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get hw_addr from an object of type ofdpa_port_experimenter_prop_scheduling.
 * @param obj Pointer to an object of type ofdpa_port_experimenter_prop_scheduling.
 * @param hw_addr Pointer to the child object of type
 * of_mac_addr_t to be filled out.
 *
 */
void
ofdpa_port_experimenter_prop_scheduling_hw_addr_get(
    ofdpa_port_experimenter_prop_scheduling_t *obj,
    of_mac_addr_t *hw_addr)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_PORT_EXPERIMENTER_PROP_SCHEDULING);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 16;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_mac_get(wbuf, abs_offset, hw_addr);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set hw_addr in an object of type ofdpa_port_experimenter_prop_scheduling.
 * @param obj Pointer to an object of type ofdpa_port_experimenter_prop_scheduling.
 * @param hw_addr The value to write into the object
 */
void
ofdpa_port_experimenter_prop_scheduling_hw_addr_set(
    ofdpa_port_experimenter_prop_scheduling_t *obj,
    of_mac_addr_t hw_addr)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_PORT_EXPERIMENTER_PROP_SCHEDULING);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 16;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_mac_set(wbuf, abs_offset, hw_addr);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get config from an object of type ofdpa_port_experimenter_prop_scheduling.
 * @param obj Pointer to an object of type ofdpa_port_experimenter_prop_scheduling.
 * @param config Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
ofdpa_port_experimenter_prop_scheduling_config_get(
    ofdpa_port_experimenter_prop_scheduling_t *obj,
    uint32_t *config)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_PORT_EXPERIMENTER_PROP_SCHEDULING);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 24;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, config);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set config in an object of type ofdpa_port_experimenter_prop_scheduling.
 * @param obj Pointer to an object of type ofdpa_port_experimenter_prop_scheduling.
 * @param config The value to write into the object
 */
void
ofdpa_port_experimenter_prop_scheduling_config_set(
    ofdpa_port_experimenter_prop_scheduling_t *obj,
    uint32_t config)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_PORT_EXPERIMENTER_PROP_SCHEDULING);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 24;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, config);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get mask from an object of type ofdpa_port_experimenter_prop_scheduling.
 * @param obj Pointer to an object of type ofdpa_port_experimenter_prop_scheduling.
 * @param mask Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
ofdpa_port_experimenter_prop_scheduling_mask_get(
    ofdpa_port_experimenter_prop_scheduling_t *obj,
    uint32_t *mask)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_PORT_EXPERIMENTER_PROP_SCHEDULING);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 28;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, mask);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set mask in an object of type ofdpa_port_experimenter_prop_scheduling.
 * @param obj Pointer to an object of type ofdpa_port_experimenter_prop_scheduling.
 * @param mask The value to write into the object
 */
void
ofdpa_port_experimenter_prop_scheduling_mask_set(
    ofdpa_port_experimenter_prop_scheduling_t *obj,
    uint32_t mask)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_PORT_EXPERIMENTER_PROP_SCHEDULING);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 28;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, mask);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get pm_type from an object of type ofdpa_port_experimenter_prop_scheduling.
 * @param obj Pointer to an object of type ofdpa_port_experimenter_prop_scheduling.
 * @param pm_type Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
ofdpa_port_experimenter_prop_scheduling_pm_type_get(
    ofdpa_port_experimenter_prop_scheduling_t *obj,
    uint16_t *pm_type)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_PORT_EXPERIMENTER_PROP_SCHEDULING);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 32;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_get(wbuf, abs_offset, pm_type);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set pm_type in an object of type ofdpa_port_experimenter_prop_scheduling.
 * @param obj Pointer to an object of type ofdpa_port_experimenter_prop_scheduling.
 * @param pm_type The value to write into the object
 */
void
ofdpa_port_experimenter_prop_scheduling_pm_type_set(
    ofdpa_port_experimenter_prop_scheduling_t *obj,
    uint16_t pm_type)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_PORT_EXPERIMENTER_PROP_SCHEDULING);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 32;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_set(wbuf, abs_offset, pm_type);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get experimenter from an object of type ofdpa_port_experimenter_prop_scheduling.
 * @param obj Pointer to an object of type ofdpa_port_experimenter_prop_scheduling.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
ofdpa_port_experimenter_prop_scheduling_experimenter_get(
    ofdpa_port_experimenter_prop_scheduling_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_PORT_EXPERIMENTER_PROP_SCHEDULING);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 36;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set experimenter in an object of type ofdpa_port_experimenter_prop_scheduling.
 * @param obj Pointer to an object of type ofdpa_port_experimenter_prop_scheduling.
 * @param experimenter The value to write into the object
 */
void
ofdpa_port_experimenter_prop_scheduling_experimenter_set(
    ofdpa_port_experimenter_prop_scheduling_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_PORT_EXPERIMENTER_PROP_SCHEDULING);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 36;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get subtype from an object of type ofdpa_port_experimenter_prop_scheduling.
 * @param obj Pointer to an object of type ofdpa_port_experimenter_prop_scheduling.
 * @param subtype Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
ofdpa_port_experimenter_prop_scheduling_subtype_get(
    ofdpa_port_experimenter_prop_scheduling_t *obj,
    uint32_t *subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_PORT_EXPERIMENTER_PROP_SCHEDULING);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 40;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set subtype in an object of type ofdpa_port_experimenter_prop_scheduling.
 * @param obj Pointer to an object of type ofdpa_port_experimenter_prop_scheduling.
 * @param subtype The value to write into the object
 */
void
ofdpa_port_experimenter_prop_scheduling_subtype_set(
    ofdpa_port_experimenter_prop_scheduling_t *obj,
    uint32_t subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_PORT_EXPERIMENTER_PROP_SCHEDULING);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 40;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get mode from an object of type ofdpa_port_experimenter_prop_scheduling.
 * @param obj Pointer to an object of type ofdpa_port_experimenter_prop_scheduling.
 * @param mode Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
ofdpa_port_experimenter_prop_scheduling_mode_get(
    ofdpa_port_experimenter_prop_scheduling_t *obj,
    uint16_t *mode)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_PORT_EXPERIMENTER_PROP_SCHEDULING);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 44;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_get(wbuf, abs_offset, mode);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set mode in an object of type ofdpa_port_experimenter_prop_scheduling.
 * @param obj Pointer to an object of type ofdpa_port_experimenter_prop_scheduling.
 * @param mode The value to write into the object
 */
void
ofdpa_port_experimenter_prop_scheduling_mode_set(
    ofdpa_port_experimenter_prop_scheduling_t *obj,
    uint16_t mode)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_PORT_EXPERIMENTER_PROP_SCHEDULING);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 44;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_set(wbuf, abs_offset, mode);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get parameter from an object of type ofdpa_port_experimenter_prop_scheduling.
 * @param obj Pointer to an object of type ofdpa_port_experimenter_prop_scheduling.
 * @param parameter Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
ofdpa_port_experimenter_prop_scheduling_parameter_get(
    ofdpa_port_experimenter_prop_scheduling_t *obj,
    uint16_t *parameter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_PORT_EXPERIMENTER_PROP_SCHEDULING);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 46;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_get(wbuf, abs_offset, parameter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set parameter in an object of type ofdpa_port_experimenter_prop_scheduling.
 * @param obj Pointer to an object of type ofdpa_port_experimenter_prop_scheduling.
 * @param parameter The value to write into the object
 */
void
ofdpa_port_experimenter_prop_scheduling_parameter_set(
    ofdpa_port_experimenter_prop_scheduling_t *obj,
    uint16_t parameter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_PORT_EXPERIMENTER_PROP_SCHEDULING);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 46;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_set(wbuf, abs_offset, parameter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}
/* Copyright (c) 2008 The Board of Trustees of The Leland Stanford Junior University */
/* Copyright (c) 2011, 2012 Open Networking Foundation */
/* Copyright (c) 2012, 2013 Big Switch Networks, Inc. */
/* See the file LICENSE.loci which should have been included in the source distribution */
#ifdef __GNUC__

#ifdef __linux__
/* glibc */
#include <features.h>
#else
/* NetBSD etc */
#include <sys/cdefs.h>
#ifdef __GNUC_PREREQ__
#define __GNUC_PREREQ __GNUC_PREREQ__
#endif
#endif

#ifndef __GNUC_PREREQ
/* fallback */
#define __GNUC_PREREQ(maj, min) 0
#endif

#if __GNUC_PREREQ(4,6)
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif

#endif

#include "loci_log.h"
#include "loci_int.h"

static void
ofdpa_queue_desc_prop_congestion_multipart_reply_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint8_t *)(buf + 0) = obj->version; /* version */
        *(uint8_t *)(buf + 1) = 0x13; /* type */
        *(uint16_t *)(buf + 8) = U16_HTON(0xffff); /* stats_type */
        *(uint32_t *)(buf + 16) = U32_HTON(0x1018); /* experimenter */
        *(uint32_t *)(buf + 20) = U32_HTON(0x13); /* subtype */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup ofdpa_queue_desc_prop_congestion_multipart_reply ofdpa_queue_desc_prop_congestion_multipart_reply
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
ofdpa_queue_desc_prop_congestion_multipart_reply_push_wire_values(ofdpa_queue_desc_prop_congestion_multipart_reply_t *obj)
{

    ofdpa_queue_desc_prop_congestion_multipart_reply_push_wire_types(obj);

    /* Message obj; set length */
    of_message_t msg;

    if ((msg = OF_OBJECT_TO_MESSAGE(obj)) != NULL) {
        of_message_length_set(msg, obj->length);
    }

    return OF_ERROR_NONE;
}

/**
 * Create a new ofdpa_queue_desc_prop_congestion_multipart_reply object
 *
 * @param version The wire version to use for the object
 * @return Pointer to the newly create object or NULL on error
 *
 * Initializes the new object with it's default fixed length associating
 * a new underlying wire buffer.
 *
 * Use new_from_message to bind an existing message to a message object,
 * or a _get function for non-message objects.
 *
 * \ingroup ofdpa_queue_desc_prop_congestion_multipart_reply
 */

ofdpa_queue_desc_prop_congestion_multipart_reply_t *
ofdpa_queue_desc_prop_congestion_multipart_reply_new(of_version_t version)
{
    ofdpa_queue_desc_prop_congestion_multipart_reply_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OFDPA_QUEUE_DESC_PROP_CONGESTION_MULTIPART_REPLY] + of_object_extra_len[version][OFDPA_QUEUE_DESC_PROP_CONGESTION_MULTIPART_REPLY];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (ofdpa_queue_desc_prop_congestion_multipart_reply_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    ofdpa_queue_desc_prop_congestion_multipart_reply_init(obj, version, bytes, 0);

    if (ofdpa_queue_desc_prop_congestion_multipart_reply_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type ofdpa_queue_desc_prop_congestion_multipart_reply.
 *
 * @param obj Pointer to the object to initialize
 * @param version The wire version to use for the object
 * @param bytes How many bytes in the object
 * @param clean_wire Boolean: If true, clear the wire object control struct
 *
 * If bytes < 0, then the default fixed length is used for the object
 *
 * This is a "coerce" function that sets up the pointers for the
 * accessors properly.
 *
 * If anything other than 0 is passed in for the buffer size, the underlying
 * wire buffer will have 'grow' called.
 */

void
ofdpa_queue_desc_prop_congestion_multipart_reply_init(ofdpa_queue_desc_prop_congestion_multipart_reply_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OFDPA_QUEUE_DESC_PROP_CONGESTION_MULTIPART_REPLY] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OFDPA_QUEUE_DESC_PROP_CONGESTION_MULTIPART_REPLY] + of_object_extra_len[version][OFDPA_QUEUE_DESC_PROP_CONGESTION_MULTIPART_REPLY];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OFDPA_QUEUE_DESC_PROP_CONGESTION_MULTIPART_REPLY;

    /* Set up the object's function pointers */

    obj->wire_type_set = ofdpa_queue_desc_prop_congestion_multipart_reply_push_wire_types;

    obj->wire_length_get = of_object_message_wire_length_get;
    obj->wire_length_set = of_object_message_wire_length_set;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Create a new ofdpa_queue_desc_prop_congestion_multipart_reply object and bind it to an existing message
 *
 * @param msg The message to bind the new object to
 * @return Pointer to the newly create object or NULL on error
 *
 * \ingroup ofdpa_queue_desc_prop_congestion_multipart_reply
 */

ofdpa_queue_desc_prop_congestion_multipart_reply_t *
ofdpa_queue_desc_prop_congestion_multipart_reply_new_from_message(of_message_t msg)
{
    ofdpa_queue_desc_prop_congestion_multipart_reply_t *obj = NULL;
    of_version_t version;
    int length;

    if (msg == NULL) return NULL;

    version = of_message_version_get(msg);
    if (!OF_VERSION_OKAY(version)) return NULL;

    length = of_message_length_get(msg);

    if ((obj = (ofdpa_queue_desc_prop_congestion_multipart_reply_t *)of_object_new(-1)) == NULL) {
        return NULL;
    }

    ofdpa_queue_desc_prop_congestion_multipart_reply_init(obj, version, 0, 0);

    if ((of_object_buffer_bind((of_object_t *)obj, OF_MESSAGE_TO_BUFFER(msg),
                               length, OF_MESSAGE_FREE_FUNCTION)) < 0) {
       FREE(obj);
       return NULL;
    }
    obj->length = length;
    obj->version = version;

    return obj;
}

/**
 * Get xid from an object of type ofdpa_queue_desc_prop_congestion_multipart_reply.
 * @param obj Pointer to an object of type ofdpa_queue_desc_prop_congestion_multipart_reply.
 * @param xid Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
ofdpa_queue_desc_prop_congestion_multipart_reply_xid_get(
    ofdpa_queue_desc_prop_congestion_multipart_reply_t *obj,
    uint32_t *xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_QUEUE_DESC_PROP_CONGESTION_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, xid);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set xid in an object of type ofdpa_queue_desc_prop_congestion_multipart_reply.
 * @param obj Pointer to an object of type ofdpa_queue_desc_prop_congestion_multipart_reply.
 * @param xid The value to write into the object
 */
void
ofdpa_queue_desc_prop_congestion_multipart_reply_xid_set(
    ofdpa_queue_desc_prop_congestion_multipart_reply_t *obj,
    uint32_t xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_QUEUE_DESC_PROP_CONGESTION_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, xid);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get flags from an object of type ofdpa_queue_desc_prop_congestion_multipart_reply.
 * @param obj Pointer to an object of type ofdpa_queue_desc_prop_congestion_multipart_reply.
 * @param flags Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
ofdpa_queue_desc_prop_congestion_multipart_reply_flags_get(
    ofdpa_queue_desc_prop_congestion_multipart_reply_t *obj,
    uint16_t *flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_QUEUE_DESC_PROP_CONGESTION_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 10;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_get(wbuf, abs_offset, flags);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set flags in an object of type ofdpa_queue_desc_prop_congestion_multipart_reply.
 * @param obj Pointer to an object of type ofdpa_queue_desc_prop_congestion_multipart_reply.
 * @param flags The value to write into the object
 */
void
ofdpa_queue_desc_prop_congestion_multipart_reply_flags_set(
    ofdpa_queue_desc_prop_congestion_multipart_reply_t *obj,
    uint16_t flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_QUEUE_DESC_PROP_CONGESTION_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 10;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_set(wbuf, abs_offset, flags);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get experimenter from an object of type ofdpa_queue_desc_prop_congestion_multipart_reply.
 * @param obj Pointer to an object of type ofdpa_queue_desc_prop_congestion_multipart_reply.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
ofdpa_queue_desc_prop_congestion_multipart_reply_experimenter_get(
    ofdpa_queue_desc_prop_congestion_multipart_reply_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_QUEUE_DESC_PROP_CONGESTION_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 16;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set experimenter in an object of type ofdpa_queue_desc_prop_congestion_multipart_reply.
 * @param obj Pointer to an object of type ofdpa_queue_desc_prop_congestion_multipart_reply.
 * @param experimenter The value to write into the object
 */
void
ofdpa_queue_desc_prop_congestion_multipart_reply_experimenter_set(
    ofdpa_queue_desc_prop_congestion_multipart_reply_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_QUEUE_DESC_PROP_CONGESTION_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 16;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get subtype from an object of type ofdpa_queue_desc_prop_congestion_multipart_reply.
 * @param obj Pointer to an object of type ofdpa_queue_desc_prop_congestion_multipart_reply.
 * @param subtype Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
ofdpa_queue_desc_prop_congestion_multipart_reply_subtype_get(
    ofdpa_queue_desc_prop_congestion_multipart_reply_t *obj,
    uint32_t *subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_QUEUE_DESC_PROP_CONGESTION_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 20;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set subtype in an object of type ofdpa_queue_desc_prop_congestion_multipart_reply.
 * @param obj Pointer to an object of type ofdpa_queue_desc_prop_congestion_multipart_reply.
 * @param subtype The value to write into the object
 */
void
ofdpa_queue_desc_prop_congestion_multipart_reply_subtype_set(
    ofdpa_queue_desc_prop_congestion_multipart_reply_t *obj,
    uint32_t subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_QUEUE_DESC_PROP_CONGESTION_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 20;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get mode from an object of type ofdpa_queue_desc_prop_congestion_multipart_reply.
 * @param obj Pointer to an object of type ofdpa_queue_desc_prop_congestion_multipart_reply.
 * @param mode Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
ofdpa_queue_desc_prop_congestion_multipart_reply_mode_get(
    ofdpa_queue_desc_prop_congestion_multipart_reply_t *obj,
    uint16_t *mode)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_QUEUE_DESC_PROP_CONGESTION_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 24;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_get(wbuf, abs_offset, mode);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set mode in an object of type ofdpa_queue_desc_prop_congestion_multipart_reply.
 * @param obj Pointer to an object of type ofdpa_queue_desc_prop_congestion_multipart_reply.
 * @param mode The value to write into the object
 */
void
ofdpa_queue_desc_prop_congestion_multipart_reply_mode_set(
    ofdpa_queue_desc_prop_congestion_multipart_reply_t *obj,
    uint16_t mode)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_QUEUE_DESC_PROP_CONGESTION_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 24;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_set(wbuf, abs_offset, mode);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}
/* Copyright (c) 2008 The Board of Trustees of The Leland Stanford Junior University */
/* Copyright (c) 2011, 2012 Open Networking Foundation */
/* Copyright (c) 2012, 2013 Big Switch Networks, Inc. */
/* See the file LICENSE.loci which should have been included in the source distribution */
#ifdef __GNUC__

#ifdef __linux__
/* glibc */
#include <features.h>
#else
/* NetBSD etc */
#include <sys/cdefs.h>
#ifdef __GNUC_PREREQ__
#define __GNUC_PREREQ __GNUC_PREREQ__
#endif
#endif

#ifndef __GNUC_PREREQ
/* fallback */
#define __GNUC_PREREQ(maj, min) 0
#endif

#if __GNUC_PREREQ(4,6)
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif

#endif

#include "loci_log.h"
#include "loci_int.h"

static void
ofdpa_queue_desc_prop_congestion_multipart_request_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint8_t *)(buf + 0) = obj->version; /* version */
        *(uint8_t *)(buf + 1) = 0x12; /* type */
        *(uint16_t *)(buf + 8) = U16_HTON(0xffff); /* stats_type */
        *(uint32_t *)(buf + 16) = U32_HTON(0x1018); /* experimenter */
        *(uint32_t *)(buf + 20) = U32_HTON(0x13); /* subtype */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup ofdpa_queue_desc_prop_congestion_multipart_request ofdpa_queue_desc_prop_congestion_multipart_request
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
ofdpa_queue_desc_prop_congestion_multipart_request_push_wire_values(ofdpa_queue_desc_prop_congestion_multipart_request_t *obj)
{

    ofdpa_queue_desc_prop_congestion_multipart_request_push_wire_types(obj);

    /* Message obj; set length */
    of_message_t msg;

    if ((msg = OF_OBJECT_TO_MESSAGE(obj)) != NULL) {
        of_message_length_set(msg, obj->length);
    }

    return OF_ERROR_NONE;
}

/**
 * Create a new ofdpa_queue_desc_prop_congestion_multipart_request object
 *
 * @param version The wire version to use for the object
 * @return Pointer to the newly create object or NULL on error
 *
 * Initializes the new object with it's default fixed length associating
 * a new underlying wire buffer.
 *
 * Use new_from_message to bind an existing message to a message object,
 * or a _get function for non-message objects.
 *
 * \ingroup ofdpa_queue_desc_prop_congestion_multipart_request
 */

ofdpa_queue_desc_prop_congestion_multipart_request_t *
ofdpa_queue_desc_prop_congestion_multipart_request_new(of_version_t version)
{
    ofdpa_queue_desc_prop_congestion_multipart_request_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OFDPA_QUEUE_DESC_PROP_CONGESTION_MULTIPART_REQUEST] + of_object_extra_len[version][OFDPA_QUEUE_DESC_PROP_CONGESTION_MULTIPART_REQUEST];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (ofdpa_queue_desc_prop_congestion_multipart_request_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    ofdpa_queue_desc_prop_congestion_multipart_request_init(obj, version, bytes, 0);

    if (ofdpa_queue_desc_prop_congestion_multipart_request_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type ofdpa_queue_desc_prop_congestion_multipart_request.
 *
 * @param obj Pointer to the object to initialize
 * @param version The wire version to use for the object
 * @param bytes How many bytes in the object
 * @param clean_wire Boolean: If true, clear the wire object control struct
 *
 * If bytes < 0, then the default fixed length is used for the object
 *
 * This is a "coerce" function that sets up the pointers for the
 * accessors properly.
 *
 * If anything other than 0 is passed in for the buffer size, the underlying
 * wire buffer will have 'grow' called.
 */

void
ofdpa_queue_desc_prop_congestion_multipart_request_init(ofdpa_queue_desc_prop_congestion_multipart_request_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OFDPA_QUEUE_DESC_PROP_CONGESTION_MULTIPART_REQUEST] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OFDPA_QUEUE_DESC_PROP_CONGESTION_MULTIPART_REQUEST] + of_object_extra_len[version][OFDPA_QUEUE_DESC_PROP_CONGESTION_MULTIPART_REQUEST];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OFDPA_QUEUE_DESC_PROP_CONGESTION_MULTIPART_REQUEST;

    /* Set up the object's function pointers */

    obj->wire_type_set = ofdpa_queue_desc_prop_congestion_multipart_request_push_wire_types;

    obj->wire_length_get = of_object_message_wire_length_get;
    obj->wire_length_set = of_object_message_wire_length_set;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Create a new ofdpa_queue_desc_prop_congestion_multipart_request object and bind it to an existing message
 *
 * @param msg The message to bind the new object to
 * @return Pointer to the newly create object or NULL on error
 *
 * \ingroup ofdpa_queue_desc_prop_congestion_multipart_request
 */

ofdpa_queue_desc_prop_congestion_multipart_request_t *
ofdpa_queue_desc_prop_congestion_multipart_request_new_from_message(of_message_t msg)
{
    ofdpa_queue_desc_prop_congestion_multipart_request_t *obj = NULL;
    of_version_t version;
    int length;

    if (msg == NULL) return NULL;

    version = of_message_version_get(msg);
    if (!OF_VERSION_OKAY(version)) return NULL;

    length = of_message_length_get(msg);

    if ((obj = (ofdpa_queue_desc_prop_congestion_multipart_request_t *)of_object_new(-1)) == NULL) {
        return NULL;
    }

    ofdpa_queue_desc_prop_congestion_multipart_request_init(obj, version, 0, 0);

    if ((of_object_buffer_bind((of_object_t *)obj, OF_MESSAGE_TO_BUFFER(msg),
                               length, OF_MESSAGE_FREE_FUNCTION)) < 0) {
       FREE(obj);
       return NULL;
    }
    obj->length = length;
    obj->version = version;

    return obj;
}

/**
 * Get xid from an object of type ofdpa_queue_desc_prop_congestion_multipart_request.
 * @param obj Pointer to an object of type ofdpa_queue_desc_prop_congestion_multipart_request.
 * @param xid Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
ofdpa_queue_desc_prop_congestion_multipart_request_xid_get(
    ofdpa_queue_desc_prop_congestion_multipart_request_t *obj,
    uint32_t *xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_QUEUE_DESC_PROP_CONGESTION_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, xid);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set xid in an object of type ofdpa_queue_desc_prop_congestion_multipart_request.
 * @param obj Pointer to an object of type ofdpa_queue_desc_prop_congestion_multipart_request.
 * @param xid The value to write into the object
 */
void
ofdpa_queue_desc_prop_congestion_multipart_request_xid_set(
    ofdpa_queue_desc_prop_congestion_multipart_request_t *obj,
    uint32_t xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_QUEUE_DESC_PROP_CONGESTION_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, xid);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get flags from an object of type ofdpa_queue_desc_prop_congestion_multipart_request.
 * @param obj Pointer to an object of type ofdpa_queue_desc_prop_congestion_multipart_request.
 * @param flags Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
ofdpa_queue_desc_prop_congestion_multipart_request_flags_get(
    ofdpa_queue_desc_prop_congestion_multipart_request_t *obj,
    uint16_t *flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_QUEUE_DESC_PROP_CONGESTION_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 10;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_get(wbuf, abs_offset, flags);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set flags in an object of type ofdpa_queue_desc_prop_congestion_multipart_request.
 * @param obj Pointer to an object of type ofdpa_queue_desc_prop_congestion_multipart_request.
 * @param flags The value to write into the object
 */
void
ofdpa_queue_desc_prop_congestion_multipart_request_flags_set(
    ofdpa_queue_desc_prop_congestion_multipart_request_t *obj,
    uint16_t flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_QUEUE_DESC_PROP_CONGESTION_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 10;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_set(wbuf, abs_offset, flags);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get experimenter from an object of type ofdpa_queue_desc_prop_congestion_multipart_request.
 * @param obj Pointer to an object of type ofdpa_queue_desc_prop_congestion_multipart_request.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
ofdpa_queue_desc_prop_congestion_multipart_request_experimenter_get(
    ofdpa_queue_desc_prop_congestion_multipart_request_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_QUEUE_DESC_PROP_CONGESTION_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 16;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set experimenter in an object of type ofdpa_queue_desc_prop_congestion_multipart_request.
 * @param obj Pointer to an object of type ofdpa_queue_desc_prop_congestion_multipart_request.
 * @param experimenter The value to write into the object
 */
void
ofdpa_queue_desc_prop_congestion_multipart_request_experimenter_set(
    ofdpa_queue_desc_prop_congestion_multipart_request_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_QUEUE_DESC_PROP_CONGESTION_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 16;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get subtype from an object of type ofdpa_queue_desc_prop_congestion_multipart_request.
 * @param obj Pointer to an object of type ofdpa_queue_desc_prop_congestion_multipart_request.
 * @param subtype Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
ofdpa_queue_desc_prop_congestion_multipart_request_subtype_get(
    ofdpa_queue_desc_prop_congestion_multipart_request_t *obj,
    uint32_t *subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_QUEUE_DESC_PROP_CONGESTION_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 20;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set subtype in an object of type ofdpa_queue_desc_prop_congestion_multipart_request.
 * @param obj Pointer to an object of type ofdpa_queue_desc_prop_congestion_multipart_request.
 * @param subtype The value to write into the object
 */
void
ofdpa_queue_desc_prop_congestion_multipart_request_subtype_set(
    ofdpa_queue_desc_prop_congestion_multipart_request_t *obj,
    uint32_t subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_QUEUE_DESC_PROP_CONGESTION_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 20;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get mode from an object of type ofdpa_queue_desc_prop_congestion_multipart_request.
 * @param obj Pointer to an object of type ofdpa_queue_desc_prop_congestion_multipart_request.
 * @param mode Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
ofdpa_queue_desc_prop_congestion_multipart_request_mode_get(
    ofdpa_queue_desc_prop_congestion_multipart_request_t *obj,
    uint16_t *mode)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_QUEUE_DESC_PROP_CONGESTION_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 24;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_get(wbuf, abs_offset, mode);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set mode in an object of type ofdpa_queue_desc_prop_congestion_multipart_request.
 * @param obj Pointer to an object of type ofdpa_queue_desc_prop_congestion_multipart_request.
 * @param mode The value to write into the object
 */
void
ofdpa_queue_desc_prop_congestion_multipart_request_mode_set(
    ofdpa_queue_desc_prop_congestion_multipart_request_t *obj,
    uint16_t mode)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_QUEUE_DESC_PROP_CONGESTION_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 24;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_set(wbuf, abs_offset, mode);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}
/* Copyright (c) 2008 The Board of Trustees of The Leland Stanford Junior University */
/* Copyright (c) 2011, 2012 Open Networking Foundation */
/* Copyright (c) 2012, 2013 Big Switch Networks, Inc. */
/* See the file LICENSE.loci which should have been included in the source distribution */
#ifdef __GNUC__

#ifdef __linux__
/* glibc */
#include <features.h>
#else
/* NetBSD etc */
#include <sys/cdefs.h>
#ifdef __GNUC_PREREQ__
#define __GNUC_PREREQ __GNUC_PREREQ__
#endif
#endif

#ifndef __GNUC_PREREQ
/* fallback */
#define __GNUC_PREREQ(maj, min) 0
#endif

#if __GNUC_PREREQ(4,6)
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif

#endif

#include "loci_log.h"
#include "loci_int.h"

static void
ofdpa_queue_desc_prop_parent_multipart_reply_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint8_t *)(buf + 0) = obj->version; /* version */
        *(uint8_t *)(buf + 1) = 0x13; /* type */
        *(uint16_t *)(buf + 8) = U16_HTON(0xffff); /* stats_type */
        *(uint32_t *)(buf + 16) = U32_HTON(0x1018); /* experimenter */
        *(uint32_t *)(buf + 20) = U32_HTON(0x12); /* subtype */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup ofdpa_queue_desc_prop_parent_multipart_reply ofdpa_queue_desc_prop_parent_multipart_reply
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
ofdpa_queue_desc_prop_parent_multipart_reply_push_wire_values(ofdpa_queue_desc_prop_parent_multipart_reply_t *obj)
{

    ofdpa_queue_desc_prop_parent_multipart_reply_push_wire_types(obj);

    /* Message obj; set length */
    of_message_t msg;

    if ((msg = OF_OBJECT_TO_MESSAGE(obj)) != NULL) {
        of_message_length_set(msg, obj->length);
    }

    return OF_ERROR_NONE;
}

/**
 * Create a new ofdpa_queue_desc_prop_parent_multipart_reply object
 *
 * @param version The wire version to use for the object
 * @return Pointer to the newly create object or NULL on error
 *
 * Initializes the new object with it's default fixed length associating
 * a new underlying wire buffer.
 *
 * Use new_from_message to bind an existing message to a message object,
 * or a _get function for non-message objects.
 *
 * \ingroup ofdpa_queue_desc_prop_parent_multipart_reply
 */

ofdpa_queue_desc_prop_parent_multipart_reply_t *
ofdpa_queue_desc_prop_parent_multipart_reply_new(of_version_t version)
{
    ofdpa_queue_desc_prop_parent_multipart_reply_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OFDPA_QUEUE_DESC_PROP_PARENT_MULTIPART_REPLY] + of_object_extra_len[version][OFDPA_QUEUE_DESC_PROP_PARENT_MULTIPART_REPLY];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (ofdpa_queue_desc_prop_parent_multipart_reply_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    ofdpa_queue_desc_prop_parent_multipart_reply_init(obj, version, bytes, 0);

    if (ofdpa_queue_desc_prop_parent_multipart_reply_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type ofdpa_queue_desc_prop_parent_multipart_reply.
 *
 * @param obj Pointer to the object to initialize
 * @param version The wire version to use for the object
 * @param bytes How many bytes in the object
 * @param clean_wire Boolean: If true, clear the wire object control struct
 *
 * If bytes < 0, then the default fixed length is used for the object
 *
 * This is a "coerce" function that sets up the pointers for the
 * accessors properly.
 *
 * If anything other than 0 is passed in for the buffer size, the underlying
 * wire buffer will have 'grow' called.
 */

void
ofdpa_queue_desc_prop_parent_multipart_reply_init(ofdpa_queue_desc_prop_parent_multipart_reply_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OFDPA_QUEUE_DESC_PROP_PARENT_MULTIPART_REPLY] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OFDPA_QUEUE_DESC_PROP_PARENT_MULTIPART_REPLY] + of_object_extra_len[version][OFDPA_QUEUE_DESC_PROP_PARENT_MULTIPART_REPLY];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OFDPA_QUEUE_DESC_PROP_PARENT_MULTIPART_REPLY;

    /* Set up the object's function pointers */

    obj->wire_type_set = ofdpa_queue_desc_prop_parent_multipart_reply_push_wire_types;

    obj->wire_length_get = of_object_message_wire_length_get;
    obj->wire_length_set = of_object_message_wire_length_set;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Create a new ofdpa_queue_desc_prop_parent_multipart_reply object and bind it to an existing message
 *
 * @param msg The message to bind the new object to
 * @return Pointer to the newly create object or NULL on error
 *
 * \ingroup ofdpa_queue_desc_prop_parent_multipart_reply
 */

ofdpa_queue_desc_prop_parent_multipart_reply_t *
ofdpa_queue_desc_prop_parent_multipart_reply_new_from_message(of_message_t msg)
{
    ofdpa_queue_desc_prop_parent_multipart_reply_t *obj = NULL;
    of_version_t version;
    int length;

    if (msg == NULL) return NULL;

    version = of_message_version_get(msg);
    if (!OF_VERSION_OKAY(version)) return NULL;

    length = of_message_length_get(msg);

    if ((obj = (ofdpa_queue_desc_prop_parent_multipart_reply_t *)of_object_new(-1)) == NULL) {
        return NULL;
    }

    ofdpa_queue_desc_prop_parent_multipart_reply_init(obj, version, 0, 0);

    if ((of_object_buffer_bind((of_object_t *)obj, OF_MESSAGE_TO_BUFFER(msg),
                               length, OF_MESSAGE_FREE_FUNCTION)) < 0) {
       FREE(obj);
       return NULL;
    }
    obj->length = length;
    obj->version = version;

    return obj;
}

/**
 * Get xid from an object of type ofdpa_queue_desc_prop_parent_multipart_reply.
 * @param obj Pointer to an object of type ofdpa_queue_desc_prop_parent_multipart_reply.
 * @param xid Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
ofdpa_queue_desc_prop_parent_multipart_reply_xid_get(
    ofdpa_queue_desc_prop_parent_multipart_reply_t *obj,
    uint32_t *xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_QUEUE_DESC_PROP_PARENT_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, xid);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set xid in an object of type ofdpa_queue_desc_prop_parent_multipart_reply.
 * @param obj Pointer to an object of type ofdpa_queue_desc_prop_parent_multipart_reply.
 * @param xid The value to write into the object
 */
void
ofdpa_queue_desc_prop_parent_multipart_reply_xid_set(
    ofdpa_queue_desc_prop_parent_multipart_reply_t *obj,
    uint32_t xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_QUEUE_DESC_PROP_PARENT_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, xid);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get flags from an object of type ofdpa_queue_desc_prop_parent_multipart_reply.
 * @param obj Pointer to an object of type ofdpa_queue_desc_prop_parent_multipart_reply.
 * @param flags Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
ofdpa_queue_desc_prop_parent_multipart_reply_flags_get(
    ofdpa_queue_desc_prop_parent_multipart_reply_t *obj,
    uint16_t *flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_QUEUE_DESC_PROP_PARENT_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 10;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_get(wbuf, abs_offset, flags);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set flags in an object of type ofdpa_queue_desc_prop_parent_multipart_reply.
 * @param obj Pointer to an object of type ofdpa_queue_desc_prop_parent_multipart_reply.
 * @param flags The value to write into the object
 */
void
ofdpa_queue_desc_prop_parent_multipart_reply_flags_set(
    ofdpa_queue_desc_prop_parent_multipart_reply_t *obj,
    uint16_t flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_QUEUE_DESC_PROP_PARENT_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 10;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_set(wbuf, abs_offset, flags);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get experimenter from an object of type ofdpa_queue_desc_prop_parent_multipart_reply.
 * @param obj Pointer to an object of type ofdpa_queue_desc_prop_parent_multipart_reply.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
ofdpa_queue_desc_prop_parent_multipart_reply_experimenter_get(
    ofdpa_queue_desc_prop_parent_multipart_reply_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_QUEUE_DESC_PROP_PARENT_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 16;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set experimenter in an object of type ofdpa_queue_desc_prop_parent_multipart_reply.
 * @param obj Pointer to an object of type ofdpa_queue_desc_prop_parent_multipart_reply.
 * @param experimenter The value to write into the object
 */
void
ofdpa_queue_desc_prop_parent_multipart_reply_experimenter_set(
    ofdpa_queue_desc_prop_parent_multipart_reply_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_QUEUE_DESC_PROP_PARENT_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 16;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get subtype from an object of type ofdpa_queue_desc_prop_parent_multipart_reply.
 * @param obj Pointer to an object of type ofdpa_queue_desc_prop_parent_multipart_reply.
 * @param subtype Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
ofdpa_queue_desc_prop_parent_multipart_reply_subtype_get(
    ofdpa_queue_desc_prop_parent_multipart_reply_t *obj,
    uint32_t *subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_QUEUE_DESC_PROP_PARENT_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 20;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set subtype in an object of type ofdpa_queue_desc_prop_parent_multipart_reply.
 * @param obj Pointer to an object of type ofdpa_queue_desc_prop_parent_multipart_reply.
 * @param subtype The value to write into the object
 */
void
ofdpa_queue_desc_prop_parent_multipart_reply_subtype_set(
    ofdpa_queue_desc_prop_parent_multipart_reply_t *obj,
    uint32_t subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_QUEUE_DESC_PROP_PARENT_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 20;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get scheduler_level from an object of type ofdpa_queue_desc_prop_parent_multipart_reply.
 * @param obj Pointer to an object of type ofdpa_queue_desc_prop_parent_multipart_reply.
 * @param scheduler_level Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
ofdpa_queue_desc_prop_parent_multipart_reply_scheduler_level_get(
    ofdpa_queue_desc_prop_parent_multipart_reply_t *obj,
    uint16_t *scheduler_level)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_QUEUE_DESC_PROP_PARENT_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 24;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_get(wbuf, abs_offset, scheduler_level);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set scheduler_level in an object of type ofdpa_queue_desc_prop_parent_multipart_reply.
 * @param obj Pointer to an object of type ofdpa_queue_desc_prop_parent_multipart_reply.
 * @param scheduler_level The value to write into the object
 */
void
ofdpa_queue_desc_prop_parent_multipart_reply_scheduler_level_set(
    ofdpa_queue_desc_prop_parent_multipart_reply_t *obj,
    uint16_t scheduler_level)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_QUEUE_DESC_PROP_PARENT_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 24;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_set(wbuf, abs_offset, scheduler_level);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}
/* Copyright (c) 2008 The Board of Trustees of The Leland Stanford Junior University */
/* Copyright (c) 2011, 2012 Open Networking Foundation */
/* Copyright (c) 2012, 2013 Big Switch Networks, Inc. */
/* See the file LICENSE.loci which should have been included in the source distribution */
#ifdef __GNUC__

#ifdef __linux__
/* glibc */
#include <features.h>
#else
/* NetBSD etc */
#include <sys/cdefs.h>
#ifdef __GNUC_PREREQ__
#define __GNUC_PREREQ __GNUC_PREREQ__
#endif
#endif

#ifndef __GNUC_PREREQ
/* fallback */
#define __GNUC_PREREQ(maj, min) 0
#endif

#if __GNUC_PREREQ(4,6)
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif

#endif

#include "loci_log.h"
#include "loci_int.h"

static void
ofdpa_queue_desc_prop_parent_multipart_request_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint8_t *)(buf + 0) = obj->version; /* version */
        *(uint8_t *)(buf + 1) = 0x12; /* type */
        *(uint16_t *)(buf + 8) = U16_HTON(0xffff); /* stats_type */
        *(uint32_t *)(buf + 16) = U32_HTON(0x1018); /* experimenter */
        *(uint32_t *)(buf + 20) = U32_HTON(0x12); /* subtype */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup ofdpa_queue_desc_prop_parent_multipart_request ofdpa_queue_desc_prop_parent_multipart_request
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
ofdpa_queue_desc_prop_parent_multipart_request_push_wire_values(ofdpa_queue_desc_prop_parent_multipart_request_t *obj)
{

    ofdpa_queue_desc_prop_parent_multipart_request_push_wire_types(obj);

    /* Message obj; set length */
    of_message_t msg;

    if ((msg = OF_OBJECT_TO_MESSAGE(obj)) != NULL) {
        of_message_length_set(msg, obj->length);
    }

    return OF_ERROR_NONE;
}

/**
 * Create a new ofdpa_queue_desc_prop_parent_multipart_request object
 *
 * @param version The wire version to use for the object
 * @return Pointer to the newly create object or NULL on error
 *
 * Initializes the new object with it's default fixed length associating
 * a new underlying wire buffer.
 *
 * Use new_from_message to bind an existing message to a message object,
 * or a _get function for non-message objects.
 *
 * \ingroup ofdpa_queue_desc_prop_parent_multipart_request
 */

ofdpa_queue_desc_prop_parent_multipart_request_t *
ofdpa_queue_desc_prop_parent_multipart_request_new(of_version_t version)
{
    ofdpa_queue_desc_prop_parent_multipart_request_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OFDPA_QUEUE_DESC_PROP_PARENT_MULTIPART_REQUEST] + of_object_extra_len[version][OFDPA_QUEUE_DESC_PROP_PARENT_MULTIPART_REQUEST];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (ofdpa_queue_desc_prop_parent_multipart_request_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    ofdpa_queue_desc_prop_parent_multipart_request_init(obj, version, bytes, 0);

    if (ofdpa_queue_desc_prop_parent_multipart_request_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type ofdpa_queue_desc_prop_parent_multipart_request.
 *
 * @param obj Pointer to the object to initialize
 * @param version The wire version to use for the object
 * @param bytes How many bytes in the object
 * @param clean_wire Boolean: If true, clear the wire object control struct
 *
 * If bytes < 0, then the default fixed length is used for the object
 *
 * This is a "coerce" function that sets up the pointers for the
 * accessors properly.
 *
 * If anything other than 0 is passed in for the buffer size, the underlying
 * wire buffer will have 'grow' called.
 */

void
ofdpa_queue_desc_prop_parent_multipart_request_init(ofdpa_queue_desc_prop_parent_multipart_request_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OFDPA_QUEUE_DESC_PROP_PARENT_MULTIPART_REQUEST] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OFDPA_QUEUE_DESC_PROP_PARENT_MULTIPART_REQUEST] + of_object_extra_len[version][OFDPA_QUEUE_DESC_PROP_PARENT_MULTIPART_REQUEST];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OFDPA_QUEUE_DESC_PROP_PARENT_MULTIPART_REQUEST;

    /* Set up the object's function pointers */

    obj->wire_type_set = ofdpa_queue_desc_prop_parent_multipart_request_push_wire_types;

    obj->wire_length_get = of_object_message_wire_length_get;
    obj->wire_length_set = of_object_message_wire_length_set;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Create a new ofdpa_queue_desc_prop_parent_multipart_request object and bind it to an existing message
 *
 * @param msg The message to bind the new object to
 * @return Pointer to the newly create object or NULL on error
 *
 * \ingroup ofdpa_queue_desc_prop_parent_multipart_request
 */

ofdpa_queue_desc_prop_parent_multipart_request_t *
ofdpa_queue_desc_prop_parent_multipart_request_new_from_message(of_message_t msg)
{
    ofdpa_queue_desc_prop_parent_multipart_request_t *obj = NULL;
    of_version_t version;
    int length;

    if (msg == NULL) return NULL;

    version = of_message_version_get(msg);
    if (!OF_VERSION_OKAY(version)) return NULL;

    length = of_message_length_get(msg);

    if ((obj = (ofdpa_queue_desc_prop_parent_multipart_request_t *)of_object_new(-1)) == NULL) {
        return NULL;
    }

    ofdpa_queue_desc_prop_parent_multipart_request_init(obj, version, 0, 0);

    if ((of_object_buffer_bind((of_object_t *)obj, OF_MESSAGE_TO_BUFFER(msg),
                               length, OF_MESSAGE_FREE_FUNCTION)) < 0) {
       FREE(obj);
       return NULL;
    }
    obj->length = length;
    obj->version = version;

    return obj;
}

/**
 * Get xid from an object of type ofdpa_queue_desc_prop_parent_multipart_request.
 * @param obj Pointer to an object of type ofdpa_queue_desc_prop_parent_multipart_request.
 * @param xid Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
ofdpa_queue_desc_prop_parent_multipart_request_xid_get(
    ofdpa_queue_desc_prop_parent_multipart_request_t *obj,
    uint32_t *xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_QUEUE_DESC_PROP_PARENT_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, xid);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set xid in an object of type ofdpa_queue_desc_prop_parent_multipart_request.
 * @param obj Pointer to an object of type ofdpa_queue_desc_prop_parent_multipart_request.
 * @param xid The value to write into the object
 */
void
ofdpa_queue_desc_prop_parent_multipart_request_xid_set(
    ofdpa_queue_desc_prop_parent_multipart_request_t *obj,
    uint32_t xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_QUEUE_DESC_PROP_PARENT_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, xid);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get flags from an object of type ofdpa_queue_desc_prop_parent_multipart_request.
 * @param obj Pointer to an object of type ofdpa_queue_desc_prop_parent_multipart_request.
 * @param flags Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
ofdpa_queue_desc_prop_parent_multipart_request_flags_get(
    ofdpa_queue_desc_prop_parent_multipart_request_t *obj,
    uint16_t *flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_QUEUE_DESC_PROP_PARENT_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 10;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_get(wbuf, abs_offset, flags);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set flags in an object of type ofdpa_queue_desc_prop_parent_multipart_request.
 * @param obj Pointer to an object of type ofdpa_queue_desc_prop_parent_multipart_request.
 * @param flags The value to write into the object
 */
void
ofdpa_queue_desc_prop_parent_multipart_request_flags_set(
    ofdpa_queue_desc_prop_parent_multipart_request_t *obj,
    uint16_t flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_QUEUE_DESC_PROP_PARENT_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 10;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_set(wbuf, abs_offset, flags);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get experimenter from an object of type ofdpa_queue_desc_prop_parent_multipart_request.
 * @param obj Pointer to an object of type ofdpa_queue_desc_prop_parent_multipart_request.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
ofdpa_queue_desc_prop_parent_multipart_request_experimenter_get(
    ofdpa_queue_desc_prop_parent_multipart_request_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_QUEUE_DESC_PROP_PARENT_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 16;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set experimenter in an object of type ofdpa_queue_desc_prop_parent_multipart_request.
 * @param obj Pointer to an object of type ofdpa_queue_desc_prop_parent_multipart_request.
 * @param experimenter The value to write into the object
 */
void
ofdpa_queue_desc_prop_parent_multipart_request_experimenter_set(
    ofdpa_queue_desc_prop_parent_multipart_request_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_QUEUE_DESC_PROP_PARENT_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 16;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get subtype from an object of type ofdpa_queue_desc_prop_parent_multipart_request.
 * @param obj Pointer to an object of type ofdpa_queue_desc_prop_parent_multipart_request.
 * @param subtype Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
ofdpa_queue_desc_prop_parent_multipart_request_subtype_get(
    ofdpa_queue_desc_prop_parent_multipart_request_t *obj,
    uint32_t *subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_QUEUE_DESC_PROP_PARENT_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 20;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set subtype in an object of type ofdpa_queue_desc_prop_parent_multipart_request.
 * @param obj Pointer to an object of type ofdpa_queue_desc_prop_parent_multipart_request.
 * @param subtype The value to write into the object
 */
void
ofdpa_queue_desc_prop_parent_multipart_request_subtype_set(
    ofdpa_queue_desc_prop_parent_multipart_request_t *obj,
    uint32_t subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_QUEUE_DESC_PROP_PARENT_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 20;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get scheduler_level from an object of type ofdpa_queue_desc_prop_parent_multipart_request.
 * @param obj Pointer to an object of type ofdpa_queue_desc_prop_parent_multipart_request.
 * @param scheduler_level Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
ofdpa_queue_desc_prop_parent_multipart_request_scheduler_level_get(
    ofdpa_queue_desc_prop_parent_multipart_request_t *obj,
    uint16_t *scheduler_level)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_QUEUE_DESC_PROP_PARENT_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 24;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_get(wbuf, abs_offset, scheduler_level);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set scheduler_level in an object of type ofdpa_queue_desc_prop_parent_multipart_request.
 * @param obj Pointer to an object of type ofdpa_queue_desc_prop_parent_multipart_request.
 * @param scheduler_level The value to write into the object
 */
void
ofdpa_queue_desc_prop_parent_multipart_request_scheduler_level_set(
    ofdpa_queue_desc_prop_parent_multipart_request_t *obj,
    uint16_t scheduler_level)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_QUEUE_DESC_PROP_PARENT_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 24;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_set(wbuf, abs_offset, scheduler_level);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}
/* Copyright (c) 2008 The Board of Trustees of The Leland Stanford Junior University */
/* Copyright (c) 2011, 2012 Open Networking Foundation */
/* Copyright (c) 2012, 2013 Big Switch Networks, Inc. */
/* See the file LICENSE.loci which should have been included in the source distribution */
#ifdef __GNUC__

#ifdef __linux__
/* glibc */
#include <features.h>
#else
/* NetBSD etc */
#include <sys/cdefs.h>
#ifdef __GNUC_PREREQ__
#define __GNUC_PREREQ __GNUC_PREREQ__
#endif
#endif

#ifndef __GNUC_PREREQ
/* fallback */
#define __GNUC_PREREQ(maj, min) 0
#endif

#if __GNUC_PREREQ(4,6)
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif

#endif

#include "loci_log.h"
#include "loci_int.h"

static void
ofdpa_queue_mod_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint8_t *)(buf + 0) = obj->version; /* version */
        *(uint8_t *)(buf + 1) = 0x4; /* type */
        *(uint32_t *)(buf + 8) = U32_HTON(0x1018); /* experimenter */
        *(uint32_t *)(buf + 12) = U32_HTON(0x11); /* subtype */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup ofdpa_queue_mod ofdpa_queue_mod
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
ofdpa_queue_mod_push_wire_values(ofdpa_queue_mod_t *obj)
{

    ofdpa_queue_mod_push_wire_types(obj);

    /* Message obj; set length */
    of_message_t msg;

    if ((msg = OF_OBJECT_TO_MESSAGE(obj)) != NULL) {
        of_message_length_set(msg, obj->length);
    }

    return OF_ERROR_NONE;
}

/**
 * Create a new ofdpa_queue_mod object
 *
 * @param version The wire version to use for the object
 * @return Pointer to the newly create object or NULL on error
 *
 * Initializes the new object with it's default fixed length associating
 * a new underlying wire buffer.
 *
 * Use new_from_message to bind an existing message to a message object,
 * or a _get function for non-message objects.
 *
 * \ingroup ofdpa_queue_mod
 */

ofdpa_queue_mod_t *
ofdpa_queue_mod_new(of_version_t version)
{
    ofdpa_queue_mod_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OFDPA_QUEUE_MOD] + of_object_extra_len[version][OFDPA_QUEUE_MOD];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (ofdpa_queue_mod_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    ofdpa_queue_mod_init(obj, version, bytes, 0);

    if (ofdpa_queue_mod_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type ofdpa_queue_mod.
 *
 * @param obj Pointer to the object to initialize
 * @param version The wire version to use for the object
 * @param bytes How many bytes in the object
 * @param clean_wire Boolean: If true, clear the wire object control struct
 *
 * If bytes < 0, then the default fixed length is used for the object
 *
 * This is a "coerce" function that sets up the pointers for the
 * accessors properly.
 *
 * If anything other than 0 is passed in for the buffer size, the underlying
 * wire buffer will have 'grow' called.
 */

void
ofdpa_queue_mod_init(ofdpa_queue_mod_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OFDPA_QUEUE_MOD] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OFDPA_QUEUE_MOD] + of_object_extra_len[version][OFDPA_QUEUE_MOD];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OFDPA_QUEUE_MOD;

    /* Set up the object's function pointers */

    obj->wire_type_set = ofdpa_queue_mod_push_wire_types;

    obj->wire_length_get = of_object_message_wire_length_get;
    obj->wire_length_set = of_object_message_wire_length_set;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Create a new ofdpa_queue_mod object and bind it to an existing message
 *
 * @param msg The message to bind the new object to
 * @return Pointer to the newly create object or NULL on error
 *
 * \ingroup ofdpa_queue_mod
 */

ofdpa_queue_mod_t *
ofdpa_queue_mod_new_from_message(of_message_t msg)
{
    ofdpa_queue_mod_t *obj = NULL;
    of_version_t version;
    int length;

    if (msg == NULL) return NULL;

    version = of_message_version_get(msg);
    if (!OF_VERSION_OKAY(version)) return NULL;

    length = of_message_length_get(msg);

    if ((obj = (ofdpa_queue_mod_t *)of_object_new(-1)) == NULL) {
        return NULL;
    }

    ofdpa_queue_mod_init(obj, version, 0, 0);

    if ((of_object_buffer_bind((of_object_t *)obj, OF_MESSAGE_TO_BUFFER(msg),
                               length, OF_MESSAGE_FREE_FUNCTION)) < 0) {
       FREE(obj);
       return NULL;
    }
    obj->length = length;
    obj->version = version;

    return obj;
}

/**
 * Get xid from an object of type ofdpa_queue_mod.
 * @param obj Pointer to an object of type ofdpa_queue_mod.
 * @param xid Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
ofdpa_queue_mod_xid_get(
    ofdpa_queue_mod_t *obj,
    uint32_t *xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_QUEUE_MOD);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, xid);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set xid in an object of type ofdpa_queue_mod.
 * @param obj Pointer to an object of type ofdpa_queue_mod.
 * @param xid The value to write into the object
 */
void
ofdpa_queue_mod_xid_set(
    ofdpa_queue_mod_t *obj,
    uint32_t xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_QUEUE_MOD);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, xid);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get experimenter from an object of type ofdpa_queue_mod.
 * @param obj Pointer to an object of type ofdpa_queue_mod.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
ofdpa_queue_mod_experimenter_get(
    ofdpa_queue_mod_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_QUEUE_MOD);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 8;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set experimenter in an object of type ofdpa_queue_mod.
 * @param obj Pointer to an object of type ofdpa_queue_mod.
 * @param experimenter The value to write into the object
 */
void
ofdpa_queue_mod_experimenter_set(
    ofdpa_queue_mod_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_QUEUE_MOD);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 8;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get subtype from an object of type ofdpa_queue_mod.
 * @param obj Pointer to an object of type ofdpa_queue_mod.
 * @param subtype Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
ofdpa_queue_mod_subtype_get(
    ofdpa_queue_mod_t *obj,
    uint32_t *subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_QUEUE_MOD);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 12;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set subtype in an object of type ofdpa_queue_mod.
 * @param obj Pointer to an object of type ofdpa_queue_mod.
 * @param subtype The value to write into the object
 */
void
ofdpa_queue_mod_subtype_set(
    ofdpa_queue_mod_t *obj,
    uint32_t subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_QUEUE_MOD);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 12;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get mod_command from an object of type ofdpa_queue_mod.
 * @param obj Pointer to an object of type ofdpa_queue_mod.
 * @param mod_command Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
ofdpa_queue_mod_mod_command_get(
    ofdpa_queue_mod_t *obj,
    uint32_t *mod_command)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_QUEUE_MOD);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 16;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, mod_command);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set mod_command in an object of type ofdpa_queue_mod.
 * @param obj Pointer to an object of type ofdpa_queue_mod.
 * @param mod_command The value to write into the object
 */
void
ofdpa_queue_mod_mod_command_set(
    ofdpa_queue_mod_t *obj,
    uint32_t mod_command)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_QUEUE_MOD);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 16;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, mod_command);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get port_no from an object of type ofdpa_queue_mod.
 * @param obj Pointer to an object of type ofdpa_queue_mod.
 * @param port_no Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
ofdpa_queue_mod_port_no_get(
    ofdpa_queue_mod_t *obj,
    uint32_t *port_no)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_QUEUE_MOD);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 26;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, port_no);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set port_no in an object of type ofdpa_queue_mod.
 * @param obj Pointer to an object of type ofdpa_queue_mod.
 * @param port_no The value to write into the object
 */
void
ofdpa_queue_mod_port_no_set(
    ofdpa_queue_mod_t *obj,
    uint32_t port_no)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_QUEUE_MOD);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 26;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, port_no);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get queue_id from an object of type ofdpa_queue_mod.
 * @param obj Pointer to an object of type ofdpa_queue_mod.
 * @param queue_id Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
ofdpa_queue_mod_queue_id_get(
    ofdpa_queue_mod_t *obj,
    uint32_t *queue_id)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_QUEUE_MOD);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 30;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, queue_id);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set queue_id in an object of type ofdpa_queue_mod.
 * @param obj Pointer to an object of type ofdpa_queue_mod.
 * @param queue_id The value to write into the object
 */
void
ofdpa_queue_mod_queue_id_set(
    ofdpa_queue_mod_t *obj,
    uint32_t queue_id)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_QUEUE_MOD);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 30;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, queue_id);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}
/* Copyright (c) 2008 The Board of Trustees of The Leland Stanford Junior University */
/* Copyright (c) 2011, 2012 Open Networking Foundation */
/* Copyright (c) 2012, 2013 Big Switch Networks, Inc. */
/* See the file LICENSE.loci which should have been included in the source distribution */
#ifdef __GNUC__

#ifdef __linux__
/* glibc */
#include <features.h>
#else
/* NetBSD etc */
#include <sys/cdefs.h>
#ifdef __GNUC_PREREQ__
#define __GNUC_PREREQ __GNUC_PREREQ__
#endif
#endif

#ifndef __GNUC_PREREQ
/* fallback */
#define __GNUC_PREREQ(maj, min) 0
#endif

#if __GNUC_PREREQ(4,6)
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif

#endif

#include "loci_log.h"
#include "loci_int.h"

static void
ofdpa_scheduler_node_mod_msg_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint8_t *)(buf + 0) = obj->version; /* version */
        *(uint8_t *)(buf + 1) = 0x4; /* type */
        *(uint32_t *)(buf + 8) = U32_HTON(0x1018); /* experimenter */
        *(uint32_t *)(buf + 12) = U32_HTON(0x14); /* subtype */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup ofdpa_scheduler_node_mod_msg ofdpa_scheduler_node_mod_msg
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
ofdpa_scheduler_node_mod_msg_push_wire_values(ofdpa_scheduler_node_mod_msg_t *obj)
{

    ofdpa_scheduler_node_mod_msg_push_wire_types(obj);

    /* Message obj; set length */
    of_message_t msg;

    if ((msg = OF_OBJECT_TO_MESSAGE(obj)) != NULL) {
        of_message_length_set(msg, obj->length);
    }

    return OF_ERROR_NONE;
}

/**
 * Create a new ofdpa_scheduler_node_mod_msg object
 *
 * @param version The wire version to use for the object
 * @return Pointer to the newly create object or NULL on error
 *
 * Initializes the new object with it's default fixed length associating
 * a new underlying wire buffer.
 *
 * Use new_from_message to bind an existing message to a message object,
 * or a _get function for non-message objects.
 *
 * \ingroup ofdpa_scheduler_node_mod_msg
 */

ofdpa_scheduler_node_mod_msg_t *
ofdpa_scheduler_node_mod_msg_new(of_version_t version)
{
    ofdpa_scheduler_node_mod_msg_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OFDPA_SCHEDULER_NODE_MOD_MSG] + of_object_extra_len[version][OFDPA_SCHEDULER_NODE_MOD_MSG];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (ofdpa_scheduler_node_mod_msg_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    ofdpa_scheduler_node_mod_msg_init(obj, version, bytes, 0);

    if (ofdpa_scheduler_node_mod_msg_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type ofdpa_scheduler_node_mod_msg.
 *
 * @param obj Pointer to the object to initialize
 * @param version The wire version to use for the object
 * @param bytes How many bytes in the object
 * @param clean_wire Boolean: If true, clear the wire object control struct
 *
 * If bytes < 0, then the default fixed length is used for the object
 *
 * This is a "coerce" function that sets up the pointers for the
 * accessors properly.
 *
 * If anything other than 0 is passed in for the buffer size, the underlying
 * wire buffer will have 'grow' called.
 */

void
ofdpa_scheduler_node_mod_msg_init(ofdpa_scheduler_node_mod_msg_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OFDPA_SCHEDULER_NODE_MOD_MSG] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OFDPA_SCHEDULER_NODE_MOD_MSG] + of_object_extra_len[version][OFDPA_SCHEDULER_NODE_MOD_MSG];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OFDPA_SCHEDULER_NODE_MOD_MSG;

    /* Set up the object's function pointers */

    obj->wire_type_set = ofdpa_scheduler_node_mod_msg_push_wire_types;

    obj->wire_length_get = of_object_message_wire_length_get;
    obj->wire_length_set = of_object_message_wire_length_set;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Create a new ofdpa_scheduler_node_mod_msg object and bind it to an existing message
 *
 * @param msg The message to bind the new object to
 * @return Pointer to the newly create object or NULL on error
 *
 * \ingroup ofdpa_scheduler_node_mod_msg
 */

ofdpa_scheduler_node_mod_msg_t *
ofdpa_scheduler_node_mod_msg_new_from_message(of_message_t msg)
{
    ofdpa_scheduler_node_mod_msg_t *obj = NULL;
    of_version_t version;
    int length;

    if (msg == NULL) return NULL;

    version = of_message_version_get(msg);
    if (!OF_VERSION_OKAY(version)) return NULL;

    length = of_message_length_get(msg);

    if ((obj = (ofdpa_scheduler_node_mod_msg_t *)of_object_new(-1)) == NULL) {
        return NULL;
    }

    ofdpa_scheduler_node_mod_msg_init(obj, version, 0, 0);

    if ((of_object_buffer_bind((of_object_t *)obj, OF_MESSAGE_TO_BUFFER(msg),
                               length, OF_MESSAGE_FREE_FUNCTION)) < 0) {
       FREE(obj);
       return NULL;
    }
    obj->length = length;
    obj->version = version;

    return obj;
}

/**
 * Get xid from an object of type ofdpa_scheduler_node_mod_msg.
 * @param obj Pointer to an object of type ofdpa_scheduler_node_mod_msg.
 * @param xid Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
ofdpa_scheduler_node_mod_msg_xid_get(
    ofdpa_scheduler_node_mod_msg_t *obj,
    uint32_t *xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_SCHEDULER_NODE_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, xid);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set xid in an object of type ofdpa_scheduler_node_mod_msg.
 * @param obj Pointer to an object of type ofdpa_scheduler_node_mod_msg.
 * @param xid The value to write into the object
 */
void
ofdpa_scheduler_node_mod_msg_xid_set(
    ofdpa_scheduler_node_mod_msg_t *obj,
    uint32_t xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_SCHEDULER_NODE_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, xid);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get experimenter from an object of type ofdpa_scheduler_node_mod_msg.
 * @param obj Pointer to an object of type ofdpa_scheduler_node_mod_msg.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
ofdpa_scheduler_node_mod_msg_experimenter_get(
    ofdpa_scheduler_node_mod_msg_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_SCHEDULER_NODE_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 8;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set experimenter in an object of type ofdpa_scheduler_node_mod_msg.
 * @param obj Pointer to an object of type ofdpa_scheduler_node_mod_msg.
 * @param experimenter The value to write into the object
 */
void
ofdpa_scheduler_node_mod_msg_experimenter_set(
    ofdpa_scheduler_node_mod_msg_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_SCHEDULER_NODE_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 8;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get subtype from an object of type ofdpa_scheduler_node_mod_msg.
 * @param obj Pointer to an object of type ofdpa_scheduler_node_mod_msg.
 * @param subtype Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
ofdpa_scheduler_node_mod_msg_subtype_get(
    ofdpa_scheduler_node_mod_msg_t *obj,
    uint32_t *subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_SCHEDULER_NODE_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 12;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set subtype in an object of type ofdpa_scheduler_node_mod_msg.
 * @param obj Pointer to an object of type ofdpa_scheduler_node_mod_msg.
 * @param subtype The value to write into the object
 */
void
ofdpa_scheduler_node_mod_msg_subtype_set(
    ofdpa_scheduler_node_mod_msg_t *obj,
    uint32_t subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_SCHEDULER_NODE_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 12;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get mod_command from an object of type ofdpa_scheduler_node_mod_msg.
 * @param obj Pointer to an object of type ofdpa_scheduler_node_mod_msg.
 * @param mod_command Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
ofdpa_scheduler_node_mod_msg_mod_command_get(
    ofdpa_scheduler_node_mod_msg_t *obj,
    uint32_t *mod_command)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_SCHEDULER_NODE_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 16;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, mod_command);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set mod_command in an object of type ofdpa_scheduler_node_mod_msg.
 * @param obj Pointer to an object of type ofdpa_scheduler_node_mod_msg.
 * @param mod_command The value to write into the object
 */
void
ofdpa_scheduler_node_mod_msg_mod_command_set(
    ofdpa_scheduler_node_mod_msg_t *obj,
    uint32_t mod_command)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_SCHEDULER_NODE_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 16;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, mod_command);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get level from an object of type ofdpa_scheduler_node_mod_msg.
 * @param obj Pointer to an object of type ofdpa_scheduler_node_mod_msg.
 * @param level Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
ofdpa_scheduler_node_mod_msg_level_get(
    ofdpa_scheduler_node_mod_msg_t *obj,
    uint16_t *level)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_SCHEDULER_NODE_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 20;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_get(wbuf, abs_offset, level);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set level in an object of type ofdpa_scheduler_node_mod_msg.
 * @param obj Pointer to an object of type ofdpa_scheduler_node_mod_msg.
 * @param level The value to write into the object
 */
void
ofdpa_scheduler_node_mod_msg_level_set(
    ofdpa_scheduler_node_mod_msg_t *obj,
    uint16_t level)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_SCHEDULER_NODE_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 20;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_set(wbuf, abs_offset, level);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get port from an object of type ofdpa_scheduler_node_mod_msg.
 * @param obj Pointer to an object of type ofdpa_scheduler_node_mod_msg.
 * @param port Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
ofdpa_scheduler_node_mod_msg_port_get(
    ofdpa_scheduler_node_mod_msg_t *obj,
    uint32_t *port)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_SCHEDULER_NODE_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 22;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, port);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set port in an object of type ofdpa_scheduler_node_mod_msg.
 * @param obj Pointer to an object of type ofdpa_scheduler_node_mod_msg.
 * @param port The value to write into the object
 */
void
ofdpa_scheduler_node_mod_msg_port_set(
    ofdpa_scheduler_node_mod_msg_t *obj,
    uint32_t port)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_SCHEDULER_NODE_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 22;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, port);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get max_rate from an object of type ofdpa_scheduler_node_mod_msg.
 * @param obj Pointer to an object of type ofdpa_scheduler_node_mod_msg.
 * @param max_rate Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
ofdpa_scheduler_node_mod_msg_max_rate_get(
    ofdpa_scheduler_node_mod_msg_t *obj,
    uint16_t *max_rate)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_SCHEDULER_NODE_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 26;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_get(wbuf, abs_offset, max_rate);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set max_rate in an object of type ofdpa_scheduler_node_mod_msg.
 * @param obj Pointer to an object of type ofdpa_scheduler_node_mod_msg.
 * @param max_rate The value to write into the object
 */
void
ofdpa_scheduler_node_mod_msg_max_rate_set(
    ofdpa_scheduler_node_mod_msg_t *obj,
    uint16_t max_rate)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_SCHEDULER_NODE_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 26;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_set(wbuf, abs_offset, max_rate);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get min_rate from an object of type ofdpa_scheduler_node_mod_msg.
 * @param obj Pointer to an object of type ofdpa_scheduler_node_mod_msg.
 * @param min_rate Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
ofdpa_scheduler_node_mod_msg_min_rate_get(
    ofdpa_scheduler_node_mod_msg_t *obj,
    uint16_t *min_rate)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_SCHEDULER_NODE_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 28;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_get(wbuf, abs_offset, min_rate);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set min_rate in an object of type ofdpa_scheduler_node_mod_msg.
 * @param obj Pointer to an object of type ofdpa_scheduler_node_mod_msg.
 * @param min_rate The value to write into the object
 */
void
ofdpa_scheduler_node_mod_msg_min_rate_set(
    ofdpa_scheduler_node_mod_msg_t *obj,
    uint16_t min_rate)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_SCHEDULER_NODE_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 28;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_set(wbuf, abs_offset, min_rate);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get mode from an object of type ofdpa_scheduler_node_mod_msg.
 * @param obj Pointer to an object of type ofdpa_scheduler_node_mod_msg.
 * @param mode Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
ofdpa_scheduler_node_mod_msg_mode_get(
    ofdpa_scheduler_node_mod_msg_t *obj,
    uint16_t *mode)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_SCHEDULER_NODE_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 30;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_get(wbuf, abs_offset, mode);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set mode in an object of type ofdpa_scheduler_node_mod_msg.
 * @param obj Pointer to an object of type ofdpa_scheduler_node_mod_msg.
 * @param mode The value to write into the object
 */
void
ofdpa_scheduler_node_mod_msg_mode_set(
    ofdpa_scheduler_node_mod_msg_t *obj,
    uint16_t mode)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_SCHEDULER_NODE_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 30;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_set(wbuf, abs_offset, mode);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get weight from an object of type ofdpa_scheduler_node_mod_msg.
 * @param obj Pointer to an object of type ofdpa_scheduler_node_mod_msg.
 * @param weight Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
ofdpa_scheduler_node_mod_msg_weight_get(
    ofdpa_scheduler_node_mod_msg_t *obj,
    uint16_t *weight)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_SCHEDULER_NODE_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 32;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_get(wbuf, abs_offset, weight);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set weight in an object of type ofdpa_scheduler_node_mod_msg.
 * @param obj Pointer to an object of type ofdpa_scheduler_node_mod_msg.
 * @param weight The value to write into the object
 */
void
ofdpa_scheduler_node_mod_msg_weight_set(
    ofdpa_scheduler_node_mod_msg_t *obj,
    uint16_t weight)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_SCHEDULER_NODE_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 32;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_set(wbuf, abs_offset, weight);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}
/* Copyright (c) 2008 The Board of Trustees of The Leland Stanford Junior University */
/* Copyright (c) 2011, 2012 Open Networking Foundation */
/* Copyright (c) 2012, 2013 Big Switch Networks, Inc. */
/* See the file LICENSE.loci which should have been included in the source distribution */
#ifdef __GNUC__

#ifdef __linux__
/* glibc */
#include <features.h>
#else
/* NetBSD etc */
#include <sys/cdefs.h>
#ifdef __GNUC_PREREQ__
#define __GNUC_PREREQ __GNUC_PREREQ__
#endif
#endif

#ifndef __GNUC_PREREQ
/* fallback */
#define __GNUC_PREREQ(maj, min) 0
#endif

#if __GNUC_PREREQ(4,6)
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif

#endif

#include "loci_log.h"
#include "loci_int.h"

static void
ofdpa_scheduler_node_multipart_request_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint8_t *)(buf + 0) = obj->version; /* version */
        *(uint8_t *)(buf + 1) = 0x12; /* type */
        *(uint16_t *)(buf + 8) = U16_HTON(0xffff); /* stats_type */
        *(uint32_t *)(buf + 16) = U32_HTON(0x1018); /* experimenter */
        *(uint32_t *)(buf + 20) = U32_HTON(0x15); /* subtype */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup ofdpa_scheduler_node_multipart_request ofdpa_scheduler_node_multipart_request
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
ofdpa_scheduler_node_multipart_request_push_wire_values(ofdpa_scheduler_node_multipart_request_t *obj)
{

    ofdpa_scheduler_node_multipart_request_push_wire_types(obj);

    /* Message obj; set length */
    of_message_t msg;

    if ((msg = OF_OBJECT_TO_MESSAGE(obj)) != NULL) {
        of_message_length_set(msg, obj->length);
    }

    return OF_ERROR_NONE;
}

/**
 * Create a new ofdpa_scheduler_node_multipart_request object
 *
 * @param version The wire version to use for the object
 * @return Pointer to the newly create object or NULL on error
 *
 * Initializes the new object with it's default fixed length associating
 * a new underlying wire buffer.
 *
 * Use new_from_message to bind an existing message to a message object,
 * or a _get function for non-message objects.
 *
 * \ingroup ofdpa_scheduler_node_multipart_request
 */

ofdpa_scheduler_node_multipart_request_t *
ofdpa_scheduler_node_multipart_request_new(of_version_t version)
{
    ofdpa_scheduler_node_multipart_request_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OFDPA_SCHEDULER_NODE_MULTIPART_REQUEST] + of_object_extra_len[version][OFDPA_SCHEDULER_NODE_MULTIPART_REQUEST];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (ofdpa_scheduler_node_multipart_request_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    ofdpa_scheduler_node_multipart_request_init(obj, version, bytes, 0);

    if (ofdpa_scheduler_node_multipart_request_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type ofdpa_scheduler_node_multipart_request.
 *
 * @param obj Pointer to the object to initialize
 * @param version The wire version to use for the object
 * @param bytes How many bytes in the object
 * @param clean_wire Boolean: If true, clear the wire object control struct
 *
 * If bytes < 0, then the default fixed length is used for the object
 *
 * This is a "coerce" function that sets up the pointers for the
 * accessors properly.
 *
 * If anything other than 0 is passed in for the buffer size, the underlying
 * wire buffer will have 'grow' called.
 */

void
ofdpa_scheduler_node_multipart_request_init(ofdpa_scheduler_node_multipart_request_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OFDPA_SCHEDULER_NODE_MULTIPART_REQUEST] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OFDPA_SCHEDULER_NODE_MULTIPART_REQUEST] + of_object_extra_len[version][OFDPA_SCHEDULER_NODE_MULTIPART_REQUEST];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OFDPA_SCHEDULER_NODE_MULTIPART_REQUEST;

    /* Set up the object's function pointers */

    obj->wire_type_set = ofdpa_scheduler_node_multipart_request_push_wire_types;

    obj->wire_length_get = of_object_message_wire_length_get;
    obj->wire_length_set = of_object_message_wire_length_set;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Create a new ofdpa_scheduler_node_multipart_request object and bind it to an existing message
 *
 * @param msg The message to bind the new object to
 * @return Pointer to the newly create object or NULL on error
 *
 * \ingroup ofdpa_scheduler_node_multipart_request
 */

ofdpa_scheduler_node_multipart_request_t *
ofdpa_scheduler_node_multipart_request_new_from_message(of_message_t msg)
{
    ofdpa_scheduler_node_multipart_request_t *obj = NULL;
    of_version_t version;
    int length;

    if (msg == NULL) return NULL;

    version = of_message_version_get(msg);
    if (!OF_VERSION_OKAY(version)) return NULL;

    length = of_message_length_get(msg);

    if ((obj = (ofdpa_scheduler_node_multipart_request_t *)of_object_new(-1)) == NULL) {
        return NULL;
    }

    ofdpa_scheduler_node_multipart_request_init(obj, version, 0, 0);

    if ((of_object_buffer_bind((of_object_t *)obj, OF_MESSAGE_TO_BUFFER(msg),
                               length, OF_MESSAGE_FREE_FUNCTION)) < 0) {
       FREE(obj);
       return NULL;
    }
    obj->length = length;
    obj->version = version;

    return obj;
}

/**
 * Get xid from an object of type ofdpa_scheduler_node_multipart_request.
 * @param obj Pointer to an object of type ofdpa_scheduler_node_multipart_request.
 * @param xid Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
ofdpa_scheduler_node_multipart_request_xid_get(
    ofdpa_scheduler_node_multipart_request_t *obj,
    uint32_t *xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_SCHEDULER_NODE_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, xid);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set xid in an object of type ofdpa_scheduler_node_multipart_request.
 * @param obj Pointer to an object of type ofdpa_scheduler_node_multipart_request.
 * @param xid The value to write into the object
 */
void
ofdpa_scheduler_node_multipart_request_xid_set(
    ofdpa_scheduler_node_multipart_request_t *obj,
    uint32_t xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_SCHEDULER_NODE_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, xid);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get flags from an object of type ofdpa_scheduler_node_multipart_request.
 * @param obj Pointer to an object of type ofdpa_scheduler_node_multipart_request.
 * @param flags Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
ofdpa_scheduler_node_multipart_request_flags_get(
    ofdpa_scheduler_node_multipart_request_t *obj,
    uint16_t *flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_SCHEDULER_NODE_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 10;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_get(wbuf, abs_offset, flags);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set flags in an object of type ofdpa_scheduler_node_multipart_request.
 * @param obj Pointer to an object of type ofdpa_scheduler_node_multipart_request.
 * @param flags The value to write into the object
 */
void
ofdpa_scheduler_node_multipart_request_flags_set(
    ofdpa_scheduler_node_multipart_request_t *obj,
    uint16_t flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_SCHEDULER_NODE_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 10;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_set(wbuf, abs_offset, flags);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get experimenter from an object of type ofdpa_scheduler_node_multipart_request.
 * @param obj Pointer to an object of type ofdpa_scheduler_node_multipart_request.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
ofdpa_scheduler_node_multipart_request_experimenter_get(
    ofdpa_scheduler_node_multipart_request_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_SCHEDULER_NODE_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 16;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set experimenter in an object of type ofdpa_scheduler_node_multipart_request.
 * @param obj Pointer to an object of type ofdpa_scheduler_node_multipart_request.
 * @param experimenter The value to write into the object
 */
void
ofdpa_scheduler_node_multipart_request_experimenter_set(
    ofdpa_scheduler_node_multipart_request_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_SCHEDULER_NODE_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 16;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get subtype from an object of type ofdpa_scheduler_node_multipart_request.
 * @param obj Pointer to an object of type ofdpa_scheduler_node_multipart_request.
 * @param subtype Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
ofdpa_scheduler_node_multipart_request_subtype_get(
    ofdpa_scheduler_node_multipart_request_t *obj,
    uint32_t *subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_SCHEDULER_NODE_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 20;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set subtype in an object of type ofdpa_scheduler_node_multipart_request.
 * @param obj Pointer to an object of type ofdpa_scheduler_node_multipart_request.
 * @param subtype The value to write into the object
 */
void
ofdpa_scheduler_node_multipart_request_subtype_set(
    ofdpa_scheduler_node_multipart_request_t *obj,
    uint32_t subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_SCHEDULER_NODE_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 20;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get port from an object of type ofdpa_scheduler_node_multipart_request.
 * @param obj Pointer to an object of type ofdpa_scheduler_node_multipart_request.
 * @param port Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
ofdpa_scheduler_node_multipart_request_port_get(
    ofdpa_scheduler_node_multipart_request_t *obj,
    uint32_t *port)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_SCHEDULER_NODE_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 24;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, port);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set port in an object of type ofdpa_scheduler_node_multipart_request.
 * @param obj Pointer to an object of type ofdpa_scheduler_node_multipart_request.
 * @param port The value to write into the object
 */
void
ofdpa_scheduler_node_multipart_request_port_set(
    ofdpa_scheduler_node_multipart_request_t *obj,
    uint32_t port)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_SCHEDULER_NODE_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 24;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, port);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get level from an object of type ofdpa_scheduler_node_multipart_request.
 * @param obj Pointer to an object of type ofdpa_scheduler_node_multipart_request.
 * @param level Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
ofdpa_scheduler_node_multipart_request_level_get(
    ofdpa_scheduler_node_multipart_request_t *obj,
    uint16_t *level)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_SCHEDULER_NODE_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 28;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_get(wbuf, abs_offset, level);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set level in an object of type ofdpa_scheduler_node_multipart_request.
 * @param obj Pointer to an object of type ofdpa_scheduler_node_multipart_request.
 * @param level The value to write into the object
 */
void
ofdpa_scheduler_node_multipart_request_level_set(
    ofdpa_scheduler_node_multipart_request_t *obj,
    uint16_t level)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_SCHEDULER_NODE_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 28;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_set(wbuf, abs_offset, level);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get max_rate from an object of type ofdpa_scheduler_node_multipart_request.
 * @param obj Pointer to an object of type ofdpa_scheduler_node_multipart_request.
 * @param max_rate Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
ofdpa_scheduler_node_multipart_request_max_rate_get(
    ofdpa_scheduler_node_multipart_request_t *obj,
    uint16_t *max_rate)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_SCHEDULER_NODE_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 30;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_get(wbuf, abs_offset, max_rate);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set max_rate in an object of type ofdpa_scheduler_node_multipart_request.
 * @param obj Pointer to an object of type ofdpa_scheduler_node_multipart_request.
 * @param max_rate The value to write into the object
 */
void
ofdpa_scheduler_node_multipart_request_max_rate_set(
    ofdpa_scheduler_node_multipart_request_t *obj,
    uint16_t max_rate)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_SCHEDULER_NODE_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 30;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_set(wbuf, abs_offset, max_rate);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get min_rate from an object of type ofdpa_scheduler_node_multipart_request.
 * @param obj Pointer to an object of type ofdpa_scheduler_node_multipart_request.
 * @param min_rate Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
ofdpa_scheduler_node_multipart_request_min_rate_get(
    ofdpa_scheduler_node_multipart_request_t *obj,
    uint16_t *min_rate)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_SCHEDULER_NODE_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 32;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_get(wbuf, abs_offset, min_rate);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set min_rate in an object of type ofdpa_scheduler_node_multipart_request.
 * @param obj Pointer to an object of type ofdpa_scheduler_node_multipart_request.
 * @param min_rate The value to write into the object
 */
void
ofdpa_scheduler_node_multipart_request_min_rate_set(
    ofdpa_scheduler_node_multipart_request_t *obj,
    uint16_t min_rate)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_SCHEDULER_NODE_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 32;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_set(wbuf, abs_offset, min_rate);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get mode from an object of type ofdpa_scheduler_node_multipart_request.
 * @param obj Pointer to an object of type ofdpa_scheduler_node_multipart_request.
 * @param mode Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
ofdpa_scheduler_node_multipart_request_mode_get(
    ofdpa_scheduler_node_multipart_request_t *obj,
    uint16_t *mode)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_SCHEDULER_NODE_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 34;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_get(wbuf, abs_offset, mode);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set mode in an object of type ofdpa_scheduler_node_multipart_request.
 * @param obj Pointer to an object of type ofdpa_scheduler_node_multipart_request.
 * @param mode The value to write into the object
 */
void
ofdpa_scheduler_node_multipart_request_mode_set(
    ofdpa_scheduler_node_multipart_request_t *obj,
    uint16_t mode)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_SCHEDULER_NODE_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 34;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_set(wbuf, abs_offset, mode);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get weight from an object of type ofdpa_scheduler_node_multipart_request.
 * @param obj Pointer to an object of type ofdpa_scheduler_node_multipart_request.
 * @param weight Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
ofdpa_scheduler_node_multipart_request_weight_get(
    ofdpa_scheduler_node_multipart_request_t *obj,
    uint16_t *weight)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_SCHEDULER_NODE_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 36;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_get(wbuf, abs_offset, weight);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set weight in an object of type ofdpa_scheduler_node_multipart_request.
 * @param obj Pointer to an object of type ofdpa_scheduler_node_multipart_request.
 * @param weight The value to write into the object
 */
void
ofdpa_scheduler_node_multipart_request_weight_set(
    ofdpa_scheduler_node_multipart_request_t *obj,
    uint16_t weight)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_SCHEDULER_NODE_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 36;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_set(wbuf, abs_offset, weight);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}
/* Copyright (c) 2008 The Board of Trustees of The Leland Stanford Junior University */
/* Copyright (c) 2011, 2012 Open Networking Foundation */
/* Copyright (c) 2012, 2013 Big Switch Networks, Inc. */
/* See the file LICENSE.loci which should have been included in the source distribution */
#ifdef __GNUC__

#ifdef __linux__
/* glibc */
#include <features.h>
#else
/* NetBSD etc */
#include <sys/cdefs.h>
#ifdef __GNUC_PREREQ__
#define __GNUC_PREREQ __GNUC_PREREQ__
#endif
#endif

#ifndef __GNUC_PREREQ
/* fallback */
#define __GNUC_PREREQ(maj, min) 0
#endif

#if __GNUC_PREREQ(4,6)
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif

#endif

#include "loci_log.h"
#include "loci_int.h"

static void
ofdpa_scheduler_node_multipart_response_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint8_t *)(buf + 0) = obj->version; /* version */
        *(uint8_t *)(buf + 1) = 0x13; /* type */
        *(uint16_t *)(buf + 8) = U16_HTON(0xffff); /* stats_type */
        *(uint32_t *)(buf + 16) = U32_HTON(0x1018); /* experimenter */
        *(uint32_t *)(buf + 20) = U32_HTON(0x15); /* subtype */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup ofdpa_scheduler_node_multipart_response ofdpa_scheduler_node_multipart_response
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
ofdpa_scheduler_node_multipart_response_push_wire_values(ofdpa_scheduler_node_multipart_response_t *obj)
{

    ofdpa_scheduler_node_multipart_response_push_wire_types(obj);

    /* Message obj; set length */
    of_message_t msg;

    if ((msg = OF_OBJECT_TO_MESSAGE(obj)) != NULL) {
        of_message_length_set(msg, obj->length);
    }

    return OF_ERROR_NONE;
}

/**
 * Create a new ofdpa_scheduler_node_multipart_response object
 *
 * @param version The wire version to use for the object
 * @return Pointer to the newly create object or NULL on error
 *
 * Initializes the new object with it's default fixed length associating
 * a new underlying wire buffer.
 *
 * Use new_from_message to bind an existing message to a message object,
 * or a _get function for non-message objects.
 *
 * \ingroup ofdpa_scheduler_node_multipart_response
 */

ofdpa_scheduler_node_multipart_response_t *
ofdpa_scheduler_node_multipart_response_new(of_version_t version)
{
    ofdpa_scheduler_node_multipart_response_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OFDPA_SCHEDULER_NODE_MULTIPART_RESPONSE] + of_object_extra_len[version][OFDPA_SCHEDULER_NODE_MULTIPART_RESPONSE];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (ofdpa_scheduler_node_multipart_response_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    ofdpa_scheduler_node_multipart_response_init(obj, version, bytes, 0);

    if (ofdpa_scheduler_node_multipart_response_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type ofdpa_scheduler_node_multipart_response.
 *
 * @param obj Pointer to the object to initialize
 * @param version The wire version to use for the object
 * @param bytes How many bytes in the object
 * @param clean_wire Boolean: If true, clear the wire object control struct
 *
 * If bytes < 0, then the default fixed length is used for the object
 *
 * This is a "coerce" function that sets up the pointers for the
 * accessors properly.
 *
 * If anything other than 0 is passed in for the buffer size, the underlying
 * wire buffer will have 'grow' called.
 */

void
ofdpa_scheduler_node_multipart_response_init(ofdpa_scheduler_node_multipart_response_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OFDPA_SCHEDULER_NODE_MULTIPART_RESPONSE] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OFDPA_SCHEDULER_NODE_MULTIPART_RESPONSE] + of_object_extra_len[version][OFDPA_SCHEDULER_NODE_MULTIPART_RESPONSE];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OFDPA_SCHEDULER_NODE_MULTIPART_RESPONSE;

    /* Set up the object's function pointers */

    obj->wire_type_set = ofdpa_scheduler_node_multipart_response_push_wire_types;

    obj->wire_length_get = of_object_message_wire_length_get;
    obj->wire_length_set = of_object_message_wire_length_set;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Create a new ofdpa_scheduler_node_multipart_response object and bind it to an existing message
 *
 * @param msg The message to bind the new object to
 * @return Pointer to the newly create object or NULL on error
 *
 * \ingroup ofdpa_scheduler_node_multipart_response
 */

ofdpa_scheduler_node_multipart_response_t *
ofdpa_scheduler_node_multipart_response_new_from_message(of_message_t msg)
{
    ofdpa_scheduler_node_multipart_response_t *obj = NULL;
    of_version_t version;
    int length;

    if (msg == NULL) return NULL;

    version = of_message_version_get(msg);
    if (!OF_VERSION_OKAY(version)) return NULL;

    length = of_message_length_get(msg);

    if ((obj = (ofdpa_scheduler_node_multipart_response_t *)of_object_new(-1)) == NULL) {
        return NULL;
    }

    ofdpa_scheduler_node_multipart_response_init(obj, version, 0, 0);

    if ((of_object_buffer_bind((of_object_t *)obj, OF_MESSAGE_TO_BUFFER(msg),
                               length, OF_MESSAGE_FREE_FUNCTION)) < 0) {
       FREE(obj);
       return NULL;
    }
    obj->length = length;
    obj->version = version;

    return obj;
}

/**
 * Get xid from an object of type ofdpa_scheduler_node_multipart_response.
 * @param obj Pointer to an object of type ofdpa_scheduler_node_multipart_response.
 * @param xid Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
ofdpa_scheduler_node_multipart_response_xid_get(
    ofdpa_scheduler_node_multipart_response_t *obj,
    uint32_t *xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_SCHEDULER_NODE_MULTIPART_RESPONSE);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, xid);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set xid in an object of type ofdpa_scheduler_node_multipart_response.
 * @param obj Pointer to an object of type ofdpa_scheduler_node_multipart_response.
 * @param xid The value to write into the object
 */
void
ofdpa_scheduler_node_multipart_response_xid_set(
    ofdpa_scheduler_node_multipart_response_t *obj,
    uint32_t xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_SCHEDULER_NODE_MULTIPART_RESPONSE);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, xid);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get flags from an object of type ofdpa_scheduler_node_multipart_response.
 * @param obj Pointer to an object of type ofdpa_scheduler_node_multipart_response.
 * @param flags Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
ofdpa_scheduler_node_multipart_response_flags_get(
    ofdpa_scheduler_node_multipart_response_t *obj,
    uint16_t *flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_SCHEDULER_NODE_MULTIPART_RESPONSE);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 10;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_get(wbuf, abs_offset, flags);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set flags in an object of type ofdpa_scheduler_node_multipart_response.
 * @param obj Pointer to an object of type ofdpa_scheduler_node_multipart_response.
 * @param flags The value to write into the object
 */
void
ofdpa_scheduler_node_multipart_response_flags_set(
    ofdpa_scheduler_node_multipart_response_t *obj,
    uint16_t flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_SCHEDULER_NODE_MULTIPART_RESPONSE);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 10;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_set(wbuf, abs_offset, flags);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get experimenter from an object of type ofdpa_scheduler_node_multipart_response.
 * @param obj Pointer to an object of type ofdpa_scheduler_node_multipart_response.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
ofdpa_scheduler_node_multipart_response_experimenter_get(
    ofdpa_scheduler_node_multipart_response_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_SCHEDULER_NODE_MULTIPART_RESPONSE);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 16;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set experimenter in an object of type ofdpa_scheduler_node_multipart_response.
 * @param obj Pointer to an object of type ofdpa_scheduler_node_multipart_response.
 * @param experimenter The value to write into the object
 */
void
ofdpa_scheduler_node_multipart_response_experimenter_set(
    ofdpa_scheduler_node_multipart_response_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_SCHEDULER_NODE_MULTIPART_RESPONSE);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 16;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get subtype from an object of type ofdpa_scheduler_node_multipart_response.
 * @param obj Pointer to an object of type ofdpa_scheduler_node_multipart_response.
 * @param subtype Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
ofdpa_scheduler_node_multipart_response_subtype_get(
    ofdpa_scheduler_node_multipart_response_t *obj,
    uint32_t *subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_SCHEDULER_NODE_MULTIPART_RESPONSE);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 20;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set subtype in an object of type ofdpa_scheduler_node_multipart_response.
 * @param obj Pointer to an object of type ofdpa_scheduler_node_multipart_response.
 * @param subtype The value to write into the object
 */
void
ofdpa_scheduler_node_multipart_response_subtype_set(
    ofdpa_scheduler_node_multipart_response_t *obj,
    uint32_t subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_SCHEDULER_NODE_MULTIPART_RESPONSE);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 20;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get port from an object of type ofdpa_scheduler_node_multipart_response.
 * @param obj Pointer to an object of type ofdpa_scheduler_node_multipart_response.
 * @param port Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
ofdpa_scheduler_node_multipart_response_port_get(
    ofdpa_scheduler_node_multipart_response_t *obj,
    uint32_t *port)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_SCHEDULER_NODE_MULTIPART_RESPONSE);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 24;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, port);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set port in an object of type ofdpa_scheduler_node_multipart_response.
 * @param obj Pointer to an object of type ofdpa_scheduler_node_multipart_response.
 * @param port The value to write into the object
 */
void
ofdpa_scheduler_node_multipart_response_port_set(
    ofdpa_scheduler_node_multipart_response_t *obj,
    uint32_t port)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_SCHEDULER_NODE_MULTIPART_RESPONSE);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 24;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, port);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get level from an object of type ofdpa_scheduler_node_multipart_response.
 * @param obj Pointer to an object of type ofdpa_scheduler_node_multipart_response.
 * @param level Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
ofdpa_scheduler_node_multipart_response_level_get(
    ofdpa_scheduler_node_multipart_response_t *obj,
    uint16_t *level)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_SCHEDULER_NODE_MULTIPART_RESPONSE);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 28;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_get(wbuf, abs_offset, level);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set level in an object of type ofdpa_scheduler_node_multipart_response.
 * @param obj Pointer to an object of type ofdpa_scheduler_node_multipart_response.
 * @param level The value to write into the object
 */
void
ofdpa_scheduler_node_multipart_response_level_set(
    ofdpa_scheduler_node_multipart_response_t *obj,
    uint16_t level)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_SCHEDULER_NODE_MULTIPART_RESPONSE);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 28;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_set(wbuf, abs_offset, level);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get max_rate from an object of type ofdpa_scheduler_node_multipart_response.
 * @param obj Pointer to an object of type ofdpa_scheduler_node_multipart_response.
 * @param max_rate Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
ofdpa_scheduler_node_multipart_response_max_rate_get(
    ofdpa_scheduler_node_multipart_response_t *obj,
    uint16_t *max_rate)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_SCHEDULER_NODE_MULTIPART_RESPONSE);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 30;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_get(wbuf, abs_offset, max_rate);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set max_rate in an object of type ofdpa_scheduler_node_multipart_response.
 * @param obj Pointer to an object of type ofdpa_scheduler_node_multipart_response.
 * @param max_rate The value to write into the object
 */
void
ofdpa_scheduler_node_multipart_response_max_rate_set(
    ofdpa_scheduler_node_multipart_response_t *obj,
    uint16_t max_rate)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_SCHEDULER_NODE_MULTIPART_RESPONSE);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 30;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_set(wbuf, abs_offset, max_rate);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get min_rate from an object of type ofdpa_scheduler_node_multipart_response.
 * @param obj Pointer to an object of type ofdpa_scheduler_node_multipart_response.
 * @param min_rate Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
ofdpa_scheduler_node_multipart_response_min_rate_get(
    ofdpa_scheduler_node_multipart_response_t *obj,
    uint16_t *min_rate)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_SCHEDULER_NODE_MULTIPART_RESPONSE);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 32;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_get(wbuf, abs_offset, min_rate);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set min_rate in an object of type ofdpa_scheduler_node_multipart_response.
 * @param obj Pointer to an object of type ofdpa_scheduler_node_multipart_response.
 * @param min_rate The value to write into the object
 */
void
ofdpa_scheduler_node_multipart_response_min_rate_set(
    ofdpa_scheduler_node_multipart_response_t *obj,
    uint16_t min_rate)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_SCHEDULER_NODE_MULTIPART_RESPONSE);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 32;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_set(wbuf, abs_offset, min_rate);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get mode from an object of type ofdpa_scheduler_node_multipart_response.
 * @param obj Pointer to an object of type ofdpa_scheduler_node_multipart_response.
 * @param mode Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
ofdpa_scheduler_node_multipart_response_mode_get(
    ofdpa_scheduler_node_multipart_response_t *obj,
    uint16_t *mode)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_SCHEDULER_NODE_MULTIPART_RESPONSE);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 34;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_get(wbuf, abs_offset, mode);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set mode in an object of type ofdpa_scheduler_node_multipart_response.
 * @param obj Pointer to an object of type ofdpa_scheduler_node_multipart_response.
 * @param mode The value to write into the object
 */
void
ofdpa_scheduler_node_multipart_response_mode_set(
    ofdpa_scheduler_node_multipart_response_t *obj,
    uint16_t mode)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_SCHEDULER_NODE_MULTIPART_RESPONSE);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 34;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_set(wbuf, abs_offset, mode);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get weight from an object of type ofdpa_scheduler_node_multipart_response.
 * @param obj Pointer to an object of type ofdpa_scheduler_node_multipart_response.
 * @param weight Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
ofdpa_scheduler_node_multipart_response_weight_get(
    ofdpa_scheduler_node_multipart_response_t *obj,
    uint16_t *weight)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_SCHEDULER_NODE_MULTIPART_RESPONSE);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 36;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_get(wbuf, abs_offset, weight);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set weight in an object of type ofdpa_scheduler_node_multipart_response.
 * @param obj Pointer to an object of type ofdpa_scheduler_node_multipart_response.
 * @param weight The value to write into the object
 */
void
ofdpa_scheduler_node_multipart_response_weight_set(
    ofdpa_scheduler_node_multipart_response_t *obj,
    uint16_t weight)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFDPA_SCHEDULER_NODE_MULTIPART_RESPONSE);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 36;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_set(wbuf, abs_offset, weight);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}
/* Copyright (c) 2008 The Board of Trustees of The Leland Stanford Junior University */
/* Copyright (c) 2011, 2012 Open Networking Foundation */
/* Copyright (c) 2012, 2013 Big Switch Networks, Inc. */
/* See the file LICENSE.loci which should have been included in the source distribution */
#ifdef __GNUC__

#ifdef __linux__
/* glibc */
#include <features.h>
#else
/* NetBSD etc */
#include <sys/cdefs.h>
#ifdef __GNUC_PREREQ__
#define __GNUC_PREREQ __GNUC_PREREQ__
#endif
#endif

#ifndef __GNUC_PREREQ
/* fallback */
#define __GNUC_PREREQ(maj, min) 0
#endif

#if __GNUC_PREREQ(4,6)
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif

#endif

#include "loci_log.h"
#include "loci_int.h"



/**
 * \defgroup ofp_queue_desc ofp_queue_desc
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
ofp_queue_desc_push_wire_values(ofp_queue_desc_t *obj)
{

    return OF_ERROR_NONE;
}

/**
 * Create a new ofp_queue_desc object
 *
 * @param version The wire version to use for the object
 * @return Pointer to the newly create object or NULL on error
 *
 * Initializes the new object with it's default fixed length associating
 * a new underlying wire buffer.
 *
 * Use new_from_message to bind an existing message to a message object,
 * or a _get function for non-message objects.
 *
 * \ingroup ofp_queue_desc
 */

ofp_queue_desc_t *
ofp_queue_desc_new(of_version_t version)
{
    ofp_queue_desc_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OFP_QUEUE_DESC] + of_object_extra_len[version][OFP_QUEUE_DESC];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (ofp_queue_desc_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    ofp_queue_desc_init(obj, version, bytes, 0);

    if (ofp_queue_desc_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type ofp_queue_desc.
 *
 * @param obj Pointer to the object to initialize
 * @param version The wire version to use for the object
 * @param bytes How many bytes in the object
 * @param clean_wire Boolean: If true, clear the wire object control struct
 *
 * If bytes < 0, then the default fixed length is used for the object
 *
 * This is a "coerce" function that sets up the pointers for the
 * accessors properly.
 *
 * If anything other than 0 is passed in for the buffer size, the underlying
 * wire buffer will have 'grow' called.
 */

void
ofp_queue_desc_init(ofp_queue_desc_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OFP_QUEUE_DESC] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OFP_QUEUE_DESC] + of_object_extra_len[version][OFP_QUEUE_DESC];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OFP_QUEUE_DESC;

    /* Set up the object's function pointers */

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Get port_no from an object of type ofp_queue_desc.
 * @param obj Pointer to an object of type ofp_queue_desc.
 * @param port_no Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
ofp_queue_desc_port_no_get(
    ofp_queue_desc_t *obj,
    uint32_t *port_no)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFP_QUEUE_DESC);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 0;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, port_no);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set port_no in an object of type ofp_queue_desc.
 * @param obj Pointer to an object of type ofp_queue_desc.
 * @param port_no The value to write into the object
 */
void
ofp_queue_desc_port_no_set(
    ofp_queue_desc_t *obj,
    uint32_t port_no)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFP_QUEUE_DESC);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 0;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, port_no);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get queue_id from an object of type ofp_queue_desc.
 * @param obj Pointer to an object of type ofp_queue_desc.
 * @param queue_id Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
ofp_queue_desc_queue_id_get(
    ofp_queue_desc_t *obj,
    uint32_t *queue_id)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFP_QUEUE_DESC);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, queue_id);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set queue_id in an object of type ofp_queue_desc.
 * @param obj Pointer to an object of type ofp_queue_desc.
 * @param queue_id The value to write into the object
 */
void
ofp_queue_desc_queue_id_set(
    ofp_queue_desc_t *obj,
    uint32_t queue_id)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFP_QUEUE_DESC);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, queue_id);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}
