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
of_action_id_ofdpa_push_l2hdr_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint16_t *)(buf + 0) = U16_HTON(0xffff); /* type */
        *(uint32_t *)(buf + 4) = U32_HTON(0x1018); /* experimenter */
        *(uint16_t *)(buf + 8) = U16_HTON(0x1); /* subtype */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_action_id_ofdpa_push_l2hdr of_action_id_ofdpa_push_l2hdr
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_action_id_ofdpa_push_l2hdr_push_wire_values(of_action_id_ofdpa_push_l2hdr_t *obj)
{

    of_action_id_ofdpa_push_l2hdr_push_wire_types(obj);

    /* TLV obj; set length */
    of_tlv16_wire_length_set((of_object_t *)obj, obj->length);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_action_id_ofdpa_push_l2hdr object
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
 * \ingroup of_action_id_ofdpa_push_l2hdr
 */

of_action_id_ofdpa_push_l2hdr_t *
of_action_id_ofdpa_push_l2hdr_new(of_version_t version)
{
    of_action_id_ofdpa_push_l2hdr_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_ACTION_ID_OFDPA_PUSH_L2HDR] + of_object_extra_len[version][OF_ACTION_ID_OFDPA_PUSH_L2HDR];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_action_id_ofdpa_push_l2hdr_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_action_id_ofdpa_push_l2hdr_init(obj, version, bytes, 0);

    if (of_action_id_ofdpa_push_l2hdr_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_action_id_ofdpa_push_l2hdr.
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
of_action_id_ofdpa_push_l2hdr_init(of_action_id_ofdpa_push_l2hdr_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_ACTION_ID_OFDPA_PUSH_L2HDR] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_ACTION_ID_OFDPA_PUSH_L2HDR] + of_object_extra_len[version][OF_ACTION_ID_OFDPA_PUSH_L2HDR];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_ACTION_ID_OFDPA_PUSH_L2HDR;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_action_id_ofdpa_push_l2hdr_push_wire_types;

    obj->wire_length_set = of_tlv16_wire_length_set;

    obj->wire_length_get = of_tlv16_wire_length_get;

    obj->wire_type_get = of_action_id_wire_object_id_get;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Get experimenter from an object of type of_action_id_ofdpa_push_l2hdr.
 * @param obj Pointer to an object of type of_action_id_ofdpa_push_l2hdr.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_action_id_ofdpa_push_l2hdr_experimenter_get(
    of_action_id_ofdpa_push_l2hdr_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_ID_OFDPA_PUSH_L2HDR);
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
    of_wire_buffer_u32_get(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set experimenter in an object of type of_action_id_ofdpa_push_l2hdr.
 * @param obj Pointer to an object of type of_action_id_ofdpa_push_l2hdr.
 * @param experimenter The value to write into the object
 */
void
of_action_id_ofdpa_push_l2hdr_experimenter_set(
    of_action_id_ofdpa_push_l2hdr_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_ID_OFDPA_PUSH_L2HDR);
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
    of_wire_buffer_u32_set(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get subtype from an object of type of_action_id_ofdpa_push_l2hdr.
 * @param obj Pointer to an object of type of_action_id_ofdpa_push_l2hdr.
 * @param subtype Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_action_id_ofdpa_push_l2hdr_subtype_get(
    of_action_id_ofdpa_push_l2hdr_t *obj,
    uint16_t *subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_ID_OFDPA_PUSH_L2HDR);
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
    of_wire_buffer_u16_get(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set subtype in an object of type of_action_id_ofdpa_push_l2hdr.
 * @param obj Pointer to an object of type of_action_id_ofdpa_push_l2hdr.
 * @param subtype The value to write into the object
 */
void
of_action_id_ofdpa_push_l2hdr_subtype_set(
    of_action_id_ofdpa_push_l2hdr_t *obj,
    uint16_t subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_ID_OFDPA_PUSH_L2HDR);
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
    of_wire_buffer_u16_set(wbuf, abs_offset, subtype);

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
of_action_ofdpa_set_counter_fields_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint16_t *)(buf + 0) = U16_HTON(0xffff); /* type */
        *(uint32_t *)(buf + 4) = U32_HTON(0x1018); /* experimenter */
        *(uint16_t *)(buf + 8) = U16_HTON(0xc); /* subtype */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_action_ofdpa_set_counter_fields of_action_ofdpa_set_counter_fields
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_action_ofdpa_set_counter_fields_push_wire_values(of_action_ofdpa_set_counter_fields_t *obj)
{

    of_action_ofdpa_set_counter_fields_push_wire_types(obj);

    /* TLV obj; set length */
    of_tlv16_wire_length_set((of_object_t *)obj, obj->length);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_action_ofdpa_set_counter_fields object
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
 * \ingroup of_action_ofdpa_set_counter_fields
 */

of_action_ofdpa_set_counter_fields_t *
of_action_ofdpa_set_counter_fields_new(of_version_t version)
{
    of_action_ofdpa_set_counter_fields_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_ACTION_OFDPA_SET_COUNTER_FIELDS] + of_object_extra_len[version][OF_ACTION_OFDPA_SET_COUNTER_FIELDS];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_action_ofdpa_set_counter_fields_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_action_ofdpa_set_counter_fields_init(obj, version, bytes, 0);

    if (of_action_ofdpa_set_counter_fields_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_action_ofdpa_set_counter_fields.
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
of_action_ofdpa_set_counter_fields_init(of_action_ofdpa_set_counter_fields_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_ACTION_OFDPA_SET_COUNTER_FIELDS] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_ACTION_OFDPA_SET_COUNTER_FIELDS] + of_object_extra_len[version][OF_ACTION_OFDPA_SET_COUNTER_FIELDS];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_ACTION_OFDPA_SET_COUNTER_FIELDS;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_action_ofdpa_set_counter_fields_push_wire_types;

    obj->wire_length_set = of_tlv16_wire_length_set;

    obj->wire_length_get = of_tlv16_wire_length_get;

    obj->wire_type_get = of_action_wire_object_id_get;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Get experimenter from an object of type of_action_ofdpa_set_counter_fields.
 * @param obj Pointer to an object of type of_action_ofdpa_set_counter_fields.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_action_ofdpa_set_counter_fields_experimenter_get(
    of_action_ofdpa_set_counter_fields_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_SET_COUNTER_FIELDS);
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
    of_wire_buffer_u32_get(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set experimenter in an object of type of_action_ofdpa_set_counter_fields.
 * @param obj Pointer to an object of type of_action_ofdpa_set_counter_fields.
 * @param experimenter The value to write into the object
 */
void
of_action_ofdpa_set_counter_fields_experimenter_set(
    of_action_ofdpa_set_counter_fields_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_SET_COUNTER_FIELDS);
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
    of_wire_buffer_u32_set(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get subtype from an object of type of_action_ofdpa_set_counter_fields.
 * @param obj Pointer to an object of type of_action_ofdpa_set_counter_fields.
 * @param subtype Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_action_ofdpa_set_counter_fields_subtype_get(
    of_action_ofdpa_set_counter_fields_t *obj,
    uint16_t *subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_SET_COUNTER_FIELDS);
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
    of_wire_buffer_u16_get(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set subtype in an object of type of_action_ofdpa_set_counter_fields.
 * @param obj Pointer to an object of type of_action_ofdpa_set_counter_fields.
 * @param subtype The value to write into the object
 */
void
of_action_ofdpa_set_counter_fields_subtype_set(
    of_action_ofdpa_set_counter_fields_t *obj,
    uint16_t subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_SET_COUNTER_FIELDS);
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
    of_wire_buffer_u16_set(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get traffic_class from an object of type of_action_ofdpa_set_counter_fields.
 * @param obj Pointer to an object of type of_action_ofdpa_set_counter_fields.
 * @param traffic_class Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_action_ofdpa_set_counter_fields_traffic_class_get(
    of_action_ofdpa_set_counter_fields_t *obj,
    uint16_t *traffic_class)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_SET_COUNTER_FIELDS);
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
    of_wire_buffer_u16_get(wbuf, abs_offset, traffic_class);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set traffic_class in an object of type of_action_ofdpa_set_counter_fields.
 * @param obj Pointer to an object of type of_action_ofdpa_set_counter_fields.
 * @param traffic_class The value to write into the object
 */
void
of_action_ofdpa_set_counter_fields_traffic_class_set(
    of_action_ofdpa_set_counter_fields_t *obj,
    uint16_t traffic_class)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_SET_COUNTER_FIELDS);
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
    of_wire_buffer_u16_set(wbuf, abs_offset, traffic_class);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get lmep_id from an object of type of_action_ofdpa_set_counter_fields.
 * @param obj Pointer to an object of type of_action_ofdpa_set_counter_fields.
 * @param lmep_id Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_action_ofdpa_set_counter_fields_lmep_id_get(
    of_action_ofdpa_set_counter_fields_t *obj,
    uint32_t *lmep_id)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_SET_COUNTER_FIELDS);
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
    of_wire_buffer_u32_get(wbuf, abs_offset, lmep_id);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set lmep_id in an object of type of_action_ofdpa_set_counter_fields.
 * @param obj Pointer to an object of type of_action_ofdpa_set_counter_fields.
 * @param lmep_id The value to write into the object
 */
void
of_action_ofdpa_set_counter_fields_lmep_id_set(
    of_action_ofdpa_set_counter_fields_t *obj,
    uint32_t lmep_id)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_SET_COUNTER_FIELDS);
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
    of_wire_buffer_u32_set(wbuf, abs_offset, lmep_id);

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
of_action_id_ofdpa_set_counter_fields_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint16_t *)(buf + 0) = U16_HTON(0xffff); /* type */
        *(uint32_t *)(buf + 4) = U32_HTON(0x1018); /* experimenter */
        *(uint16_t *)(buf + 8) = U16_HTON(0xc); /* subtype */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_action_id_ofdpa_set_counter_fields of_action_id_ofdpa_set_counter_fields
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_action_id_ofdpa_set_counter_fields_push_wire_values(of_action_id_ofdpa_set_counter_fields_t *obj)
{

    of_action_id_ofdpa_set_counter_fields_push_wire_types(obj);

    /* TLV obj; set length */
    of_tlv16_wire_length_set((of_object_t *)obj, obj->length);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_action_id_ofdpa_set_counter_fields object
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
 * \ingroup of_action_id_ofdpa_set_counter_fields
 */

of_action_id_ofdpa_set_counter_fields_t *
of_action_id_ofdpa_set_counter_fields_new(of_version_t version)
{
    of_action_id_ofdpa_set_counter_fields_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_ACTION_ID_OFDPA_SET_COUNTER_FIELDS] + of_object_extra_len[version][OF_ACTION_ID_OFDPA_SET_COUNTER_FIELDS];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_action_id_ofdpa_set_counter_fields_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_action_id_ofdpa_set_counter_fields_init(obj, version, bytes, 0);

    if (of_action_id_ofdpa_set_counter_fields_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_action_id_ofdpa_set_counter_fields.
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
of_action_id_ofdpa_set_counter_fields_init(of_action_id_ofdpa_set_counter_fields_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_ACTION_ID_OFDPA_SET_COUNTER_FIELDS] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_ACTION_ID_OFDPA_SET_COUNTER_FIELDS] + of_object_extra_len[version][OF_ACTION_ID_OFDPA_SET_COUNTER_FIELDS];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_ACTION_ID_OFDPA_SET_COUNTER_FIELDS;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_action_id_ofdpa_set_counter_fields_push_wire_types;

    obj->wire_length_set = of_tlv16_wire_length_set;

    obj->wire_length_get = of_tlv16_wire_length_get;

    obj->wire_type_get = of_action_id_wire_object_id_get;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Get experimenter from an object of type of_action_id_ofdpa_set_counter_fields.
 * @param obj Pointer to an object of type of_action_id_ofdpa_set_counter_fields.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_action_id_ofdpa_set_counter_fields_experimenter_get(
    of_action_id_ofdpa_set_counter_fields_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_ID_OFDPA_SET_COUNTER_FIELDS);
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
    of_wire_buffer_u32_get(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set experimenter in an object of type of_action_id_ofdpa_set_counter_fields.
 * @param obj Pointer to an object of type of_action_id_ofdpa_set_counter_fields.
 * @param experimenter The value to write into the object
 */
void
of_action_id_ofdpa_set_counter_fields_experimenter_set(
    of_action_id_ofdpa_set_counter_fields_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_ID_OFDPA_SET_COUNTER_FIELDS);
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
    of_wire_buffer_u32_set(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get subtype from an object of type of_action_id_ofdpa_set_counter_fields.
 * @param obj Pointer to an object of type of_action_id_ofdpa_set_counter_fields.
 * @param subtype Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_action_id_ofdpa_set_counter_fields_subtype_get(
    of_action_id_ofdpa_set_counter_fields_t *obj,
    uint16_t *subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_ID_OFDPA_SET_COUNTER_FIELDS);
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
    of_wire_buffer_u16_get(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set subtype in an object of type of_action_id_ofdpa_set_counter_fields.
 * @param obj Pointer to an object of type of_action_id_ofdpa_set_counter_fields.
 * @param subtype The value to write into the object
 */
void
of_action_id_ofdpa_set_counter_fields_subtype_set(
    of_action_id_ofdpa_set_counter_fields_t *obj,
    uint16_t subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_ID_OFDPA_SET_COUNTER_FIELDS);
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
    of_wire_buffer_u16_set(wbuf, abs_offset, subtype);

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
of_action_ofdpa_set_mpls_pcpdei_from_table_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint16_t *)(buf + 0) = U16_HTON(0xffff); /* type */
        *(uint32_t *)(buf + 4) = U32_HTON(0x1018); /* experimenter */
        *(uint16_t *)(buf + 8) = U16_HTON(0x10); /* subtype */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_action_ofdpa_set_mpls_pcpdei_from_table of_action_ofdpa_set_mpls_pcpdei_from_table
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_action_ofdpa_set_mpls_pcpdei_from_table_push_wire_values(of_action_ofdpa_set_mpls_pcpdei_from_table_t *obj)
{

    of_action_ofdpa_set_mpls_pcpdei_from_table_push_wire_types(obj);

    /* TLV obj; set length */
    of_tlv16_wire_length_set((of_object_t *)obj, obj->length);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_action_ofdpa_set_mpls_pcpdei_from_table object
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
 * \ingroup of_action_ofdpa_set_mpls_pcpdei_from_table
 */

of_action_ofdpa_set_mpls_pcpdei_from_table_t *
of_action_ofdpa_set_mpls_pcpdei_from_table_new(of_version_t version)
{
    of_action_ofdpa_set_mpls_pcpdei_from_table_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_ACTION_OFDPA_SET_MPLS_PCPDEI_FROM_TABLE] + of_object_extra_len[version][OF_ACTION_OFDPA_SET_MPLS_PCPDEI_FROM_TABLE];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_action_ofdpa_set_mpls_pcpdei_from_table_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_action_ofdpa_set_mpls_pcpdei_from_table_init(obj, version, bytes, 0);

    if (of_action_ofdpa_set_mpls_pcpdei_from_table_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_action_ofdpa_set_mpls_pcpdei_from_table.
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
of_action_ofdpa_set_mpls_pcpdei_from_table_init(of_action_ofdpa_set_mpls_pcpdei_from_table_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_ACTION_OFDPA_SET_MPLS_PCPDEI_FROM_TABLE] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_ACTION_OFDPA_SET_MPLS_PCPDEI_FROM_TABLE] + of_object_extra_len[version][OF_ACTION_OFDPA_SET_MPLS_PCPDEI_FROM_TABLE];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_ACTION_OFDPA_SET_MPLS_PCPDEI_FROM_TABLE;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_action_ofdpa_set_mpls_pcpdei_from_table_push_wire_types;

    obj->wire_length_set = of_tlv16_wire_length_set;

    obj->wire_length_get = of_tlv16_wire_length_get;

    obj->wire_type_get = of_action_wire_object_id_get;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Get experimenter from an object of type of_action_ofdpa_set_mpls_pcpdei_from_table.
 * @param obj Pointer to an object of type of_action_ofdpa_set_mpls_pcpdei_from_table.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_action_ofdpa_set_mpls_pcpdei_from_table_experimenter_get(
    of_action_ofdpa_set_mpls_pcpdei_from_table_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_SET_MPLS_PCPDEI_FROM_TABLE);
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
    of_wire_buffer_u32_get(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set experimenter in an object of type of_action_ofdpa_set_mpls_pcpdei_from_table.
 * @param obj Pointer to an object of type of_action_ofdpa_set_mpls_pcpdei_from_table.
 * @param experimenter The value to write into the object
 */
void
of_action_ofdpa_set_mpls_pcpdei_from_table_experimenter_set(
    of_action_ofdpa_set_mpls_pcpdei_from_table_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_SET_MPLS_PCPDEI_FROM_TABLE);
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
    of_wire_buffer_u32_set(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get subtype from an object of type of_action_ofdpa_set_mpls_pcpdei_from_table.
 * @param obj Pointer to an object of type of_action_ofdpa_set_mpls_pcpdei_from_table.
 * @param subtype Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_action_ofdpa_set_mpls_pcpdei_from_table_subtype_get(
    of_action_ofdpa_set_mpls_pcpdei_from_table_t *obj,
    uint16_t *subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_SET_MPLS_PCPDEI_FROM_TABLE);
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
    of_wire_buffer_u16_get(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set subtype in an object of type of_action_ofdpa_set_mpls_pcpdei_from_table.
 * @param obj Pointer to an object of type of_action_ofdpa_set_mpls_pcpdei_from_table.
 * @param subtype The value to write into the object
 */
void
of_action_ofdpa_set_mpls_pcpdei_from_table_subtype_set(
    of_action_ofdpa_set_mpls_pcpdei_from_table_t *obj,
    uint16_t subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_SET_MPLS_PCPDEI_FROM_TABLE);
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
    of_wire_buffer_u16_set(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get qos_index from an object of type of_action_ofdpa_set_mpls_pcpdei_from_table.
 * @param obj Pointer to an object of type of_action_ofdpa_set_mpls_pcpdei_from_table.
 * @param qos_index Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
of_action_ofdpa_set_mpls_pcpdei_from_table_qos_index_get(
    of_action_ofdpa_set_mpls_pcpdei_from_table_t *obj,
    uint8_t *qos_index)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_SET_MPLS_PCPDEI_FROM_TABLE);
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
    of_wire_buffer_u8_get(wbuf, abs_offset, qos_index);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set qos_index in an object of type of_action_ofdpa_set_mpls_pcpdei_from_table.
 * @param obj Pointer to an object of type of_action_ofdpa_set_mpls_pcpdei_from_table.
 * @param qos_index The value to write into the object
 */
void
of_action_ofdpa_set_mpls_pcpdei_from_table_qos_index_set(
    of_action_ofdpa_set_mpls_pcpdei_from_table_t *obj,
    uint8_t qos_index)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_SET_MPLS_PCPDEI_FROM_TABLE);
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
    of_wire_buffer_u8_set(wbuf, abs_offset, qos_index);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get traffic_class from an object of type of_action_ofdpa_set_mpls_pcpdei_from_table.
 * @param obj Pointer to an object of type of_action_ofdpa_set_mpls_pcpdei_from_table.
 * @param traffic_class Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
of_action_ofdpa_set_mpls_pcpdei_from_table_traffic_class_get(
    of_action_ofdpa_set_mpls_pcpdei_from_table_t *obj,
    uint8_t *traffic_class)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_SET_MPLS_PCPDEI_FROM_TABLE);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 11;
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
 * Set traffic_class in an object of type of_action_ofdpa_set_mpls_pcpdei_from_table.
 * @param obj Pointer to an object of type of_action_ofdpa_set_mpls_pcpdei_from_table.
 * @param traffic_class The value to write into the object
 */
void
of_action_ofdpa_set_mpls_pcpdei_from_table_traffic_class_set(
    of_action_ofdpa_set_mpls_pcpdei_from_table_t *obj,
    uint8_t traffic_class)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_SET_MPLS_PCPDEI_FROM_TABLE);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 11;
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
 * Get color from an object of type of_action_ofdpa_set_mpls_pcpdei_from_table.
 * @param obj Pointer to an object of type of_action_ofdpa_set_mpls_pcpdei_from_table.
 * @param color Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
of_action_ofdpa_set_mpls_pcpdei_from_table_color_get(
    of_action_ofdpa_set_mpls_pcpdei_from_table_t *obj,
    uint8_t *color)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_SET_MPLS_PCPDEI_FROM_TABLE);
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
    of_wire_buffer_u8_get(wbuf, abs_offset, color);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set color in an object of type of_action_ofdpa_set_mpls_pcpdei_from_table.
 * @param obj Pointer to an object of type of_action_ofdpa_set_mpls_pcpdei_from_table.
 * @param color The value to write into the object
 */
void
of_action_ofdpa_set_mpls_pcpdei_from_table_color_set(
    of_action_ofdpa_set_mpls_pcpdei_from_table_t *obj,
    uint8_t color)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_SET_MPLS_PCPDEI_FROM_TABLE);
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
    of_wire_buffer_u8_set(wbuf, abs_offset, color);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get pcp_dei from an object of type of_action_ofdpa_set_mpls_pcpdei_from_table.
 * @param obj Pointer to an object of type of_action_ofdpa_set_mpls_pcpdei_from_table.
 * @param pcp_dei Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
of_action_ofdpa_set_mpls_pcpdei_from_table_pcp_dei_get(
    of_action_ofdpa_set_mpls_pcpdei_from_table_t *obj,
    uint8_t *pcp_dei)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_SET_MPLS_PCPDEI_FROM_TABLE);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 13;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_get(wbuf, abs_offset, pcp_dei);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set pcp_dei in an object of type of_action_ofdpa_set_mpls_pcpdei_from_table.
 * @param obj Pointer to an object of type of_action_ofdpa_set_mpls_pcpdei_from_table.
 * @param pcp_dei The value to write into the object
 */
void
of_action_ofdpa_set_mpls_pcpdei_from_table_pcp_dei_set(
    of_action_ofdpa_set_mpls_pcpdei_from_table_t *obj,
    uint8_t pcp_dei)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_SET_MPLS_PCPDEI_FROM_TABLE);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 13;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_set(wbuf, abs_offset, pcp_dei);

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
of_action_id_ofdpa_set_mpls_pcpdei_from_table_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint16_t *)(buf + 0) = U16_HTON(0xffff); /* type */
        *(uint32_t *)(buf + 4) = U32_HTON(0x1018); /* experimenter */
        *(uint16_t *)(buf + 8) = U16_HTON(0x10); /* subtype */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_action_id_ofdpa_set_mpls_pcpdei_from_table of_action_id_ofdpa_set_mpls_pcpdei_from_table
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_action_id_ofdpa_set_mpls_pcpdei_from_table_push_wire_values(of_action_id_ofdpa_set_mpls_pcpdei_from_table_t *obj)
{

    of_action_id_ofdpa_set_mpls_pcpdei_from_table_push_wire_types(obj);

    /* TLV obj; set length */
    of_tlv16_wire_length_set((of_object_t *)obj, obj->length);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_action_id_ofdpa_set_mpls_pcpdei_from_table object
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
 * \ingroup of_action_id_ofdpa_set_mpls_pcpdei_from_table
 */

of_action_id_ofdpa_set_mpls_pcpdei_from_table_t *
of_action_id_ofdpa_set_mpls_pcpdei_from_table_new(of_version_t version)
{
    of_action_id_ofdpa_set_mpls_pcpdei_from_table_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_ACTION_ID_OFDPA_SET_MPLS_PCPDEI_FROM_TABLE] + of_object_extra_len[version][OF_ACTION_ID_OFDPA_SET_MPLS_PCPDEI_FROM_TABLE];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_action_id_ofdpa_set_mpls_pcpdei_from_table_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_action_id_ofdpa_set_mpls_pcpdei_from_table_init(obj, version, bytes, 0);

    if (of_action_id_ofdpa_set_mpls_pcpdei_from_table_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_action_id_ofdpa_set_mpls_pcpdei_from_table.
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
of_action_id_ofdpa_set_mpls_pcpdei_from_table_init(of_action_id_ofdpa_set_mpls_pcpdei_from_table_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_ACTION_ID_OFDPA_SET_MPLS_PCPDEI_FROM_TABLE] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_ACTION_ID_OFDPA_SET_MPLS_PCPDEI_FROM_TABLE] + of_object_extra_len[version][OF_ACTION_ID_OFDPA_SET_MPLS_PCPDEI_FROM_TABLE];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_ACTION_ID_OFDPA_SET_MPLS_PCPDEI_FROM_TABLE;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_action_id_ofdpa_set_mpls_pcpdei_from_table_push_wire_types;

    obj->wire_length_set = of_tlv16_wire_length_set;

    obj->wire_length_get = of_tlv16_wire_length_get;

    obj->wire_type_get = of_action_id_wire_object_id_get;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Get experimenter from an object of type of_action_id_ofdpa_set_mpls_pcpdei_from_table.
 * @param obj Pointer to an object of type of_action_id_ofdpa_set_mpls_pcpdei_from_table.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_action_id_ofdpa_set_mpls_pcpdei_from_table_experimenter_get(
    of_action_id_ofdpa_set_mpls_pcpdei_from_table_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_ID_OFDPA_SET_MPLS_PCPDEI_FROM_TABLE);
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
    of_wire_buffer_u32_get(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set experimenter in an object of type of_action_id_ofdpa_set_mpls_pcpdei_from_table.
 * @param obj Pointer to an object of type of_action_id_ofdpa_set_mpls_pcpdei_from_table.
 * @param experimenter The value to write into the object
 */
void
of_action_id_ofdpa_set_mpls_pcpdei_from_table_experimenter_set(
    of_action_id_ofdpa_set_mpls_pcpdei_from_table_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_ID_OFDPA_SET_MPLS_PCPDEI_FROM_TABLE);
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
    of_wire_buffer_u32_set(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get subtype from an object of type of_action_id_ofdpa_set_mpls_pcpdei_from_table.
 * @param obj Pointer to an object of type of_action_id_ofdpa_set_mpls_pcpdei_from_table.
 * @param subtype Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_action_id_ofdpa_set_mpls_pcpdei_from_table_subtype_get(
    of_action_id_ofdpa_set_mpls_pcpdei_from_table_t *obj,
    uint16_t *subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_ID_OFDPA_SET_MPLS_PCPDEI_FROM_TABLE);
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
    of_wire_buffer_u16_get(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set subtype in an object of type of_action_id_ofdpa_set_mpls_pcpdei_from_table.
 * @param obj Pointer to an object of type of_action_id_ofdpa_set_mpls_pcpdei_from_table.
 * @param subtype The value to write into the object
 */
void
of_action_id_ofdpa_set_mpls_pcpdei_from_table_subtype_set(
    of_action_id_ofdpa_set_mpls_pcpdei_from_table_t *obj,
    uint16_t subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_ID_OFDPA_SET_MPLS_PCPDEI_FROM_TABLE);
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
    of_wire_buffer_u16_set(wbuf, abs_offset, subtype);

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
of_action_ofdpa_set_mpls_pcpdei_from_tunnel_table_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint16_t *)(buf + 0) = U16_HTON(0xffff); /* type */
        *(uint32_t *)(buf + 4) = U32_HTON(0x1018); /* experimenter */
        *(uint16_t *)(buf + 8) = U16_HTON(0x12); /* subtype */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_action_ofdpa_set_mpls_pcpdei_from_tunnel_table of_action_ofdpa_set_mpls_pcpdei_from_tunnel_table
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_action_ofdpa_set_mpls_pcpdei_from_tunnel_table_push_wire_values(of_action_ofdpa_set_mpls_pcpdei_from_tunnel_table_t *obj)
{

    of_action_ofdpa_set_mpls_pcpdei_from_tunnel_table_push_wire_types(obj);

    /* TLV obj; set length */
    of_tlv16_wire_length_set((of_object_t *)obj, obj->length);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_action_ofdpa_set_mpls_pcpdei_from_tunnel_table object
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
 * \ingroup of_action_ofdpa_set_mpls_pcpdei_from_tunnel_table
 */

of_action_ofdpa_set_mpls_pcpdei_from_tunnel_table_t *
of_action_ofdpa_set_mpls_pcpdei_from_tunnel_table_new(of_version_t version)
{
    of_action_ofdpa_set_mpls_pcpdei_from_tunnel_table_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_ACTION_OFDPA_SET_MPLS_PCPDEI_FROM_TUNNEL_TABLE] + of_object_extra_len[version][OF_ACTION_OFDPA_SET_MPLS_PCPDEI_FROM_TUNNEL_TABLE];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_action_ofdpa_set_mpls_pcpdei_from_tunnel_table_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_action_ofdpa_set_mpls_pcpdei_from_tunnel_table_init(obj, version, bytes, 0);

    if (of_action_ofdpa_set_mpls_pcpdei_from_tunnel_table_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_action_ofdpa_set_mpls_pcpdei_from_tunnel_table.
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
of_action_ofdpa_set_mpls_pcpdei_from_tunnel_table_init(of_action_ofdpa_set_mpls_pcpdei_from_tunnel_table_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_ACTION_OFDPA_SET_MPLS_PCPDEI_FROM_TUNNEL_TABLE] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_ACTION_OFDPA_SET_MPLS_PCPDEI_FROM_TUNNEL_TABLE] + of_object_extra_len[version][OF_ACTION_OFDPA_SET_MPLS_PCPDEI_FROM_TUNNEL_TABLE];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_ACTION_OFDPA_SET_MPLS_PCPDEI_FROM_TUNNEL_TABLE;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_action_ofdpa_set_mpls_pcpdei_from_tunnel_table_push_wire_types;

    obj->wire_length_set = of_tlv16_wire_length_set;

    obj->wire_length_get = of_tlv16_wire_length_get;

    obj->wire_type_get = of_action_wire_object_id_get;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Get experimenter from an object of type of_action_ofdpa_set_mpls_pcpdei_from_tunnel_table.
 * @param obj Pointer to an object of type of_action_ofdpa_set_mpls_pcpdei_from_tunnel_table.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_action_ofdpa_set_mpls_pcpdei_from_tunnel_table_experimenter_get(
    of_action_ofdpa_set_mpls_pcpdei_from_tunnel_table_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_SET_MPLS_PCPDEI_FROM_TUNNEL_TABLE);
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
    of_wire_buffer_u32_get(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set experimenter in an object of type of_action_ofdpa_set_mpls_pcpdei_from_tunnel_table.
 * @param obj Pointer to an object of type of_action_ofdpa_set_mpls_pcpdei_from_tunnel_table.
 * @param experimenter The value to write into the object
 */
void
of_action_ofdpa_set_mpls_pcpdei_from_tunnel_table_experimenter_set(
    of_action_ofdpa_set_mpls_pcpdei_from_tunnel_table_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_SET_MPLS_PCPDEI_FROM_TUNNEL_TABLE);
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
    of_wire_buffer_u32_set(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get subtype from an object of type of_action_ofdpa_set_mpls_pcpdei_from_tunnel_table.
 * @param obj Pointer to an object of type of_action_ofdpa_set_mpls_pcpdei_from_tunnel_table.
 * @param subtype Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_action_ofdpa_set_mpls_pcpdei_from_tunnel_table_subtype_get(
    of_action_ofdpa_set_mpls_pcpdei_from_tunnel_table_t *obj,
    uint16_t *subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_SET_MPLS_PCPDEI_FROM_TUNNEL_TABLE);
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
    of_wire_buffer_u16_get(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set subtype in an object of type of_action_ofdpa_set_mpls_pcpdei_from_tunnel_table.
 * @param obj Pointer to an object of type of_action_ofdpa_set_mpls_pcpdei_from_tunnel_table.
 * @param subtype The value to write into the object
 */
void
of_action_ofdpa_set_mpls_pcpdei_from_tunnel_table_subtype_set(
    of_action_ofdpa_set_mpls_pcpdei_from_tunnel_table_t *obj,
    uint16_t subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_SET_MPLS_PCPDEI_FROM_TUNNEL_TABLE);
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
    of_wire_buffer_u16_set(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get qos_index from an object of type of_action_ofdpa_set_mpls_pcpdei_from_tunnel_table.
 * @param obj Pointer to an object of type of_action_ofdpa_set_mpls_pcpdei_from_tunnel_table.
 * @param qos_index Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
of_action_ofdpa_set_mpls_pcpdei_from_tunnel_table_qos_index_get(
    of_action_ofdpa_set_mpls_pcpdei_from_tunnel_table_t *obj,
    uint8_t *qos_index)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_SET_MPLS_PCPDEI_FROM_TUNNEL_TABLE);
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
    of_wire_buffer_u8_get(wbuf, abs_offset, qos_index);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set qos_index in an object of type of_action_ofdpa_set_mpls_pcpdei_from_tunnel_table.
 * @param obj Pointer to an object of type of_action_ofdpa_set_mpls_pcpdei_from_tunnel_table.
 * @param qos_index The value to write into the object
 */
void
of_action_ofdpa_set_mpls_pcpdei_from_tunnel_table_qos_index_set(
    of_action_ofdpa_set_mpls_pcpdei_from_tunnel_table_t *obj,
    uint8_t qos_index)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_SET_MPLS_PCPDEI_FROM_TUNNEL_TABLE);
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
    of_wire_buffer_u8_set(wbuf, abs_offset, qos_index);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get traffic_class from an object of type of_action_ofdpa_set_mpls_pcpdei_from_tunnel_table.
 * @param obj Pointer to an object of type of_action_ofdpa_set_mpls_pcpdei_from_tunnel_table.
 * @param traffic_class Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
of_action_ofdpa_set_mpls_pcpdei_from_tunnel_table_traffic_class_get(
    of_action_ofdpa_set_mpls_pcpdei_from_tunnel_table_t *obj,
    uint8_t *traffic_class)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_SET_MPLS_PCPDEI_FROM_TUNNEL_TABLE);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 11;
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
 * Set traffic_class in an object of type of_action_ofdpa_set_mpls_pcpdei_from_tunnel_table.
 * @param obj Pointer to an object of type of_action_ofdpa_set_mpls_pcpdei_from_tunnel_table.
 * @param traffic_class The value to write into the object
 */
void
of_action_ofdpa_set_mpls_pcpdei_from_tunnel_table_traffic_class_set(
    of_action_ofdpa_set_mpls_pcpdei_from_tunnel_table_t *obj,
    uint8_t traffic_class)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_SET_MPLS_PCPDEI_FROM_TUNNEL_TABLE);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 11;
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
 * Get color from an object of type of_action_ofdpa_set_mpls_pcpdei_from_tunnel_table.
 * @param obj Pointer to an object of type of_action_ofdpa_set_mpls_pcpdei_from_tunnel_table.
 * @param color Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
of_action_ofdpa_set_mpls_pcpdei_from_tunnel_table_color_get(
    of_action_ofdpa_set_mpls_pcpdei_from_tunnel_table_t *obj,
    uint8_t *color)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_SET_MPLS_PCPDEI_FROM_TUNNEL_TABLE);
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
    of_wire_buffer_u8_get(wbuf, abs_offset, color);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set color in an object of type of_action_ofdpa_set_mpls_pcpdei_from_tunnel_table.
 * @param obj Pointer to an object of type of_action_ofdpa_set_mpls_pcpdei_from_tunnel_table.
 * @param color The value to write into the object
 */
void
of_action_ofdpa_set_mpls_pcpdei_from_tunnel_table_color_set(
    of_action_ofdpa_set_mpls_pcpdei_from_tunnel_table_t *obj,
    uint8_t color)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_SET_MPLS_PCPDEI_FROM_TUNNEL_TABLE);
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
    of_wire_buffer_u8_set(wbuf, abs_offset, color);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get pcp_dei from an object of type of_action_ofdpa_set_mpls_pcpdei_from_tunnel_table.
 * @param obj Pointer to an object of type of_action_ofdpa_set_mpls_pcpdei_from_tunnel_table.
 * @param pcp_dei Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
of_action_ofdpa_set_mpls_pcpdei_from_tunnel_table_pcp_dei_get(
    of_action_ofdpa_set_mpls_pcpdei_from_tunnel_table_t *obj,
    uint8_t *pcp_dei)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_SET_MPLS_PCPDEI_FROM_TUNNEL_TABLE);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 13;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_get(wbuf, abs_offset, pcp_dei);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set pcp_dei in an object of type of_action_ofdpa_set_mpls_pcpdei_from_tunnel_table.
 * @param obj Pointer to an object of type of_action_ofdpa_set_mpls_pcpdei_from_tunnel_table.
 * @param pcp_dei The value to write into the object
 */
void
of_action_ofdpa_set_mpls_pcpdei_from_tunnel_table_pcp_dei_set(
    of_action_ofdpa_set_mpls_pcpdei_from_tunnel_table_t *obj,
    uint8_t pcp_dei)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_SET_MPLS_PCPDEI_FROM_TUNNEL_TABLE);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 13;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_set(wbuf, abs_offset, pcp_dei);

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
of_action_id_ofdpa_set_mpls_pcpdei_from_tunnel_table_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint16_t *)(buf + 0) = U16_HTON(0xffff); /* type */
        *(uint32_t *)(buf + 4) = U32_HTON(0x1018); /* experimenter */
        *(uint16_t *)(buf + 8) = U16_HTON(0x12); /* subtype */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_action_id_ofdpa_set_mpls_pcpdei_from_tunnel_table of_action_id_ofdpa_set_mpls_pcpdei_from_tunnel_table
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_action_id_ofdpa_set_mpls_pcpdei_from_tunnel_table_push_wire_values(of_action_id_ofdpa_set_mpls_pcpdei_from_tunnel_table_t *obj)
{

    of_action_id_ofdpa_set_mpls_pcpdei_from_tunnel_table_push_wire_types(obj);

    /* TLV obj; set length */
    of_tlv16_wire_length_set((of_object_t *)obj, obj->length);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_action_id_ofdpa_set_mpls_pcpdei_from_tunnel_table object
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
 * \ingroup of_action_id_ofdpa_set_mpls_pcpdei_from_tunnel_table
 */

of_action_id_ofdpa_set_mpls_pcpdei_from_tunnel_table_t *
of_action_id_ofdpa_set_mpls_pcpdei_from_tunnel_table_new(of_version_t version)
{
    of_action_id_ofdpa_set_mpls_pcpdei_from_tunnel_table_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_ACTION_ID_OFDPA_SET_MPLS_PCPDEI_FROM_TUNNEL_TABLE] + of_object_extra_len[version][OF_ACTION_ID_OFDPA_SET_MPLS_PCPDEI_FROM_TUNNEL_TABLE];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_action_id_ofdpa_set_mpls_pcpdei_from_tunnel_table_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_action_id_ofdpa_set_mpls_pcpdei_from_tunnel_table_init(obj, version, bytes, 0);

    if (of_action_id_ofdpa_set_mpls_pcpdei_from_tunnel_table_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_action_id_ofdpa_set_mpls_pcpdei_from_tunnel_table.
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
of_action_id_ofdpa_set_mpls_pcpdei_from_tunnel_table_init(of_action_id_ofdpa_set_mpls_pcpdei_from_tunnel_table_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_ACTION_ID_OFDPA_SET_MPLS_PCPDEI_FROM_TUNNEL_TABLE] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_ACTION_ID_OFDPA_SET_MPLS_PCPDEI_FROM_TUNNEL_TABLE] + of_object_extra_len[version][OF_ACTION_ID_OFDPA_SET_MPLS_PCPDEI_FROM_TUNNEL_TABLE];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_ACTION_ID_OFDPA_SET_MPLS_PCPDEI_FROM_TUNNEL_TABLE;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_action_id_ofdpa_set_mpls_pcpdei_from_tunnel_table_push_wire_types;

    obj->wire_length_set = of_tlv16_wire_length_set;

    obj->wire_length_get = of_tlv16_wire_length_get;

    obj->wire_type_get = of_action_id_wire_object_id_get;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Get experimenter from an object of type of_action_id_ofdpa_set_mpls_pcpdei_from_tunnel_table.
 * @param obj Pointer to an object of type of_action_id_ofdpa_set_mpls_pcpdei_from_tunnel_table.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_action_id_ofdpa_set_mpls_pcpdei_from_tunnel_table_experimenter_get(
    of_action_id_ofdpa_set_mpls_pcpdei_from_tunnel_table_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_ID_OFDPA_SET_MPLS_PCPDEI_FROM_TUNNEL_TABLE);
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
    of_wire_buffer_u32_get(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set experimenter in an object of type of_action_id_ofdpa_set_mpls_pcpdei_from_tunnel_table.
 * @param obj Pointer to an object of type of_action_id_ofdpa_set_mpls_pcpdei_from_tunnel_table.
 * @param experimenter The value to write into the object
 */
void
of_action_id_ofdpa_set_mpls_pcpdei_from_tunnel_table_experimenter_set(
    of_action_id_ofdpa_set_mpls_pcpdei_from_tunnel_table_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_ID_OFDPA_SET_MPLS_PCPDEI_FROM_TUNNEL_TABLE);
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
    of_wire_buffer_u32_set(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get subtype from an object of type of_action_id_ofdpa_set_mpls_pcpdei_from_tunnel_table.
 * @param obj Pointer to an object of type of_action_id_ofdpa_set_mpls_pcpdei_from_tunnel_table.
 * @param subtype Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_action_id_ofdpa_set_mpls_pcpdei_from_tunnel_table_subtype_get(
    of_action_id_ofdpa_set_mpls_pcpdei_from_tunnel_table_t *obj,
    uint16_t *subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_ID_OFDPA_SET_MPLS_PCPDEI_FROM_TUNNEL_TABLE);
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
    of_wire_buffer_u16_get(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set subtype in an object of type of_action_id_ofdpa_set_mpls_pcpdei_from_tunnel_table.
 * @param obj Pointer to an object of type of_action_id_ofdpa_set_mpls_pcpdei_from_tunnel_table.
 * @param subtype The value to write into the object
 */
void
of_action_id_ofdpa_set_mpls_pcpdei_from_tunnel_table_subtype_set(
    of_action_id_ofdpa_set_mpls_pcpdei_from_tunnel_table_t *obj,
    uint16_t subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_ID_OFDPA_SET_MPLS_PCPDEI_FROM_TUNNEL_TABLE);
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
    of_wire_buffer_u16_set(wbuf, abs_offset, subtype);

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
of_action_ofdpa_set_mpls_qos_from_table_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint16_t *)(buf + 0) = U16_HTON(0xffff); /* type */
        *(uint32_t *)(buf + 4) = U32_HTON(0x1018); /* experimenter */
        *(uint16_t *)(buf + 8) = U16_HTON(0xf); /* subtype */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_action_ofdpa_set_mpls_qos_from_table of_action_ofdpa_set_mpls_qos_from_table
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_action_ofdpa_set_mpls_qos_from_table_push_wire_values(of_action_ofdpa_set_mpls_qos_from_table_t *obj)
{

    of_action_ofdpa_set_mpls_qos_from_table_push_wire_types(obj);

    /* TLV obj; set length */
    of_tlv16_wire_length_set((of_object_t *)obj, obj->length);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_action_ofdpa_set_mpls_qos_from_table object
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
 * \ingroup of_action_ofdpa_set_mpls_qos_from_table
 */

of_action_ofdpa_set_mpls_qos_from_table_t *
of_action_ofdpa_set_mpls_qos_from_table_new(of_version_t version)
{
    of_action_ofdpa_set_mpls_qos_from_table_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_ACTION_OFDPA_SET_MPLS_QOS_FROM_TABLE] + of_object_extra_len[version][OF_ACTION_OFDPA_SET_MPLS_QOS_FROM_TABLE];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_action_ofdpa_set_mpls_qos_from_table_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_action_ofdpa_set_mpls_qos_from_table_init(obj, version, bytes, 0);

    if (of_action_ofdpa_set_mpls_qos_from_table_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_action_ofdpa_set_mpls_qos_from_table.
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
of_action_ofdpa_set_mpls_qos_from_table_init(of_action_ofdpa_set_mpls_qos_from_table_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_ACTION_OFDPA_SET_MPLS_QOS_FROM_TABLE] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_ACTION_OFDPA_SET_MPLS_QOS_FROM_TABLE] + of_object_extra_len[version][OF_ACTION_OFDPA_SET_MPLS_QOS_FROM_TABLE];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_ACTION_OFDPA_SET_MPLS_QOS_FROM_TABLE;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_action_ofdpa_set_mpls_qos_from_table_push_wire_types;

    obj->wire_length_set = of_tlv16_wire_length_set;

    obj->wire_length_get = of_tlv16_wire_length_get;

    obj->wire_type_get = of_action_wire_object_id_get;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Get experimenter from an object of type of_action_ofdpa_set_mpls_qos_from_table.
 * @param obj Pointer to an object of type of_action_ofdpa_set_mpls_qos_from_table.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_action_ofdpa_set_mpls_qos_from_table_experimenter_get(
    of_action_ofdpa_set_mpls_qos_from_table_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_SET_MPLS_QOS_FROM_TABLE);
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
    of_wire_buffer_u32_get(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set experimenter in an object of type of_action_ofdpa_set_mpls_qos_from_table.
 * @param obj Pointer to an object of type of_action_ofdpa_set_mpls_qos_from_table.
 * @param experimenter The value to write into the object
 */
void
of_action_ofdpa_set_mpls_qos_from_table_experimenter_set(
    of_action_ofdpa_set_mpls_qos_from_table_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_SET_MPLS_QOS_FROM_TABLE);
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
    of_wire_buffer_u32_set(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get subtype from an object of type of_action_ofdpa_set_mpls_qos_from_table.
 * @param obj Pointer to an object of type of_action_ofdpa_set_mpls_qos_from_table.
 * @param subtype Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_action_ofdpa_set_mpls_qos_from_table_subtype_get(
    of_action_ofdpa_set_mpls_qos_from_table_t *obj,
    uint16_t *subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_SET_MPLS_QOS_FROM_TABLE);
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
    of_wire_buffer_u16_get(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set subtype in an object of type of_action_ofdpa_set_mpls_qos_from_table.
 * @param obj Pointer to an object of type of_action_ofdpa_set_mpls_qos_from_table.
 * @param subtype The value to write into the object
 */
void
of_action_ofdpa_set_mpls_qos_from_table_subtype_set(
    of_action_ofdpa_set_mpls_qos_from_table_t *obj,
    uint16_t subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_SET_MPLS_QOS_FROM_TABLE);
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
    of_wire_buffer_u16_set(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get qos_index from an object of type of_action_ofdpa_set_mpls_qos_from_table.
 * @param obj Pointer to an object of type of_action_ofdpa_set_mpls_qos_from_table.
 * @param qos_index Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
of_action_ofdpa_set_mpls_qos_from_table_qos_index_get(
    of_action_ofdpa_set_mpls_qos_from_table_t *obj,
    uint8_t *qos_index)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_SET_MPLS_QOS_FROM_TABLE);
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
    of_wire_buffer_u8_get(wbuf, abs_offset, qos_index);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set qos_index in an object of type of_action_ofdpa_set_mpls_qos_from_table.
 * @param obj Pointer to an object of type of_action_ofdpa_set_mpls_qos_from_table.
 * @param qos_index The value to write into the object
 */
void
of_action_ofdpa_set_mpls_qos_from_table_qos_index_set(
    of_action_ofdpa_set_mpls_qos_from_table_t *obj,
    uint8_t qos_index)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_SET_MPLS_QOS_FROM_TABLE);
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
    of_wire_buffer_u8_set(wbuf, abs_offset, qos_index);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get mpls_tc from an object of type of_action_ofdpa_set_mpls_qos_from_table.
 * @param obj Pointer to an object of type of_action_ofdpa_set_mpls_qos_from_table.
 * @param mpls_tc Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
of_action_ofdpa_set_mpls_qos_from_table_mpls_tc_get(
    of_action_ofdpa_set_mpls_qos_from_table_t *obj,
    uint8_t *mpls_tc)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_SET_MPLS_QOS_FROM_TABLE);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 11;
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
 * Set mpls_tc in an object of type of_action_ofdpa_set_mpls_qos_from_table.
 * @param obj Pointer to an object of type of_action_ofdpa_set_mpls_qos_from_table.
 * @param mpls_tc The value to write into the object
 */
void
of_action_ofdpa_set_mpls_qos_from_table_mpls_tc_set(
    of_action_ofdpa_set_mpls_qos_from_table_t *obj,
    uint8_t mpls_tc)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_SET_MPLS_QOS_FROM_TABLE);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 11;
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
of_action_id_ofdpa_set_mpls_qos_from_table_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint16_t *)(buf + 0) = U16_HTON(0xffff); /* type */
        *(uint32_t *)(buf + 4) = U32_HTON(0x1018); /* experimenter */
        *(uint16_t *)(buf + 8) = U16_HTON(0xf); /* subtype */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_action_id_ofdpa_set_mpls_qos_from_table of_action_id_ofdpa_set_mpls_qos_from_table
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_action_id_ofdpa_set_mpls_qos_from_table_push_wire_values(of_action_id_ofdpa_set_mpls_qos_from_table_t *obj)
{

    of_action_id_ofdpa_set_mpls_qos_from_table_push_wire_types(obj);

    /* TLV obj; set length */
    of_tlv16_wire_length_set((of_object_t *)obj, obj->length);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_action_id_ofdpa_set_mpls_qos_from_table object
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
 * \ingroup of_action_id_ofdpa_set_mpls_qos_from_table
 */

of_action_id_ofdpa_set_mpls_qos_from_table_t *
of_action_id_ofdpa_set_mpls_qos_from_table_new(of_version_t version)
{
    of_action_id_ofdpa_set_mpls_qos_from_table_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_ACTION_ID_OFDPA_SET_MPLS_QOS_FROM_TABLE] + of_object_extra_len[version][OF_ACTION_ID_OFDPA_SET_MPLS_QOS_FROM_TABLE];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_action_id_ofdpa_set_mpls_qos_from_table_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_action_id_ofdpa_set_mpls_qos_from_table_init(obj, version, bytes, 0);

    if (of_action_id_ofdpa_set_mpls_qos_from_table_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_action_id_ofdpa_set_mpls_qos_from_table.
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
of_action_id_ofdpa_set_mpls_qos_from_table_init(of_action_id_ofdpa_set_mpls_qos_from_table_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_ACTION_ID_OFDPA_SET_MPLS_QOS_FROM_TABLE] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_ACTION_ID_OFDPA_SET_MPLS_QOS_FROM_TABLE] + of_object_extra_len[version][OF_ACTION_ID_OFDPA_SET_MPLS_QOS_FROM_TABLE];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_ACTION_ID_OFDPA_SET_MPLS_QOS_FROM_TABLE;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_action_id_ofdpa_set_mpls_qos_from_table_push_wire_types;

    obj->wire_length_set = of_tlv16_wire_length_set;

    obj->wire_length_get = of_tlv16_wire_length_get;

    obj->wire_type_get = of_action_id_wire_object_id_get;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Get experimenter from an object of type of_action_id_ofdpa_set_mpls_qos_from_table.
 * @param obj Pointer to an object of type of_action_id_ofdpa_set_mpls_qos_from_table.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_action_id_ofdpa_set_mpls_qos_from_table_experimenter_get(
    of_action_id_ofdpa_set_mpls_qos_from_table_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_ID_OFDPA_SET_MPLS_QOS_FROM_TABLE);
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
    of_wire_buffer_u32_get(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set experimenter in an object of type of_action_id_ofdpa_set_mpls_qos_from_table.
 * @param obj Pointer to an object of type of_action_id_ofdpa_set_mpls_qos_from_table.
 * @param experimenter The value to write into the object
 */
void
of_action_id_ofdpa_set_mpls_qos_from_table_experimenter_set(
    of_action_id_ofdpa_set_mpls_qos_from_table_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_ID_OFDPA_SET_MPLS_QOS_FROM_TABLE);
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
    of_wire_buffer_u32_set(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get subtype from an object of type of_action_id_ofdpa_set_mpls_qos_from_table.
 * @param obj Pointer to an object of type of_action_id_ofdpa_set_mpls_qos_from_table.
 * @param subtype Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_action_id_ofdpa_set_mpls_qos_from_table_subtype_get(
    of_action_id_ofdpa_set_mpls_qos_from_table_t *obj,
    uint16_t *subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_ID_OFDPA_SET_MPLS_QOS_FROM_TABLE);
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
    of_wire_buffer_u16_get(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set subtype in an object of type of_action_id_ofdpa_set_mpls_qos_from_table.
 * @param obj Pointer to an object of type of_action_id_ofdpa_set_mpls_qos_from_table.
 * @param subtype The value to write into the object
 */
void
of_action_id_ofdpa_set_mpls_qos_from_table_subtype_set(
    of_action_id_ofdpa_set_mpls_qos_from_table_t *obj,
    uint16_t subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_ID_OFDPA_SET_MPLS_QOS_FROM_TABLE);
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
    of_wire_buffer_u16_set(wbuf, abs_offset, subtype);

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
of_action_ofdpa_set_mpls_tc_from_tunnel_table_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint16_t *)(buf + 0) = U16_HTON(0xffff); /* type */
        *(uint32_t *)(buf + 4) = U32_HTON(0x1018); /* experimenter */
        *(uint16_t *)(buf + 8) = U16_HTON(0x11); /* subtype */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_action_ofdpa_set_mpls_tc_from_tunnel_table of_action_ofdpa_set_mpls_tc_from_tunnel_table
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_action_ofdpa_set_mpls_tc_from_tunnel_table_push_wire_values(of_action_ofdpa_set_mpls_tc_from_tunnel_table_t *obj)
{

    of_action_ofdpa_set_mpls_tc_from_tunnel_table_push_wire_types(obj);

    /* TLV obj; set length */
    of_tlv16_wire_length_set((of_object_t *)obj, obj->length);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_action_ofdpa_set_mpls_tc_from_tunnel_table object
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
 * \ingroup of_action_ofdpa_set_mpls_tc_from_tunnel_table
 */

of_action_ofdpa_set_mpls_tc_from_tunnel_table_t *
of_action_ofdpa_set_mpls_tc_from_tunnel_table_new(of_version_t version)
{
    of_action_ofdpa_set_mpls_tc_from_tunnel_table_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_ACTION_OFDPA_SET_MPLS_TC_FROM_TUNNEL_TABLE] + of_object_extra_len[version][OF_ACTION_OFDPA_SET_MPLS_TC_FROM_TUNNEL_TABLE];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_action_ofdpa_set_mpls_tc_from_tunnel_table_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_action_ofdpa_set_mpls_tc_from_tunnel_table_init(obj, version, bytes, 0);

    if (of_action_ofdpa_set_mpls_tc_from_tunnel_table_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_action_ofdpa_set_mpls_tc_from_tunnel_table.
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
of_action_ofdpa_set_mpls_tc_from_tunnel_table_init(of_action_ofdpa_set_mpls_tc_from_tunnel_table_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_ACTION_OFDPA_SET_MPLS_TC_FROM_TUNNEL_TABLE] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_ACTION_OFDPA_SET_MPLS_TC_FROM_TUNNEL_TABLE] + of_object_extra_len[version][OF_ACTION_OFDPA_SET_MPLS_TC_FROM_TUNNEL_TABLE];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_ACTION_OFDPA_SET_MPLS_TC_FROM_TUNNEL_TABLE;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_action_ofdpa_set_mpls_tc_from_tunnel_table_push_wire_types;

    obj->wire_length_set = of_tlv16_wire_length_set;

    obj->wire_length_get = of_tlv16_wire_length_get;

    obj->wire_type_get = of_action_wire_object_id_get;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Get experimenter from an object of type of_action_ofdpa_set_mpls_tc_from_tunnel_table.
 * @param obj Pointer to an object of type of_action_ofdpa_set_mpls_tc_from_tunnel_table.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_action_ofdpa_set_mpls_tc_from_tunnel_table_experimenter_get(
    of_action_ofdpa_set_mpls_tc_from_tunnel_table_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_SET_MPLS_TC_FROM_TUNNEL_TABLE);
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
    of_wire_buffer_u32_get(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set experimenter in an object of type of_action_ofdpa_set_mpls_tc_from_tunnel_table.
 * @param obj Pointer to an object of type of_action_ofdpa_set_mpls_tc_from_tunnel_table.
 * @param experimenter The value to write into the object
 */
void
of_action_ofdpa_set_mpls_tc_from_tunnel_table_experimenter_set(
    of_action_ofdpa_set_mpls_tc_from_tunnel_table_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_SET_MPLS_TC_FROM_TUNNEL_TABLE);
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
    of_wire_buffer_u32_set(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get subtype from an object of type of_action_ofdpa_set_mpls_tc_from_tunnel_table.
 * @param obj Pointer to an object of type of_action_ofdpa_set_mpls_tc_from_tunnel_table.
 * @param subtype Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_action_ofdpa_set_mpls_tc_from_tunnel_table_subtype_get(
    of_action_ofdpa_set_mpls_tc_from_tunnel_table_t *obj,
    uint16_t *subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_SET_MPLS_TC_FROM_TUNNEL_TABLE);
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
    of_wire_buffer_u16_get(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set subtype in an object of type of_action_ofdpa_set_mpls_tc_from_tunnel_table.
 * @param obj Pointer to an object of type of_action_ofdpa_set_mpls_tc_from_tunnel_table.
 * @param subtype The value to write into the object
 */
void
of_action_ofdpa_set_mpls_tc_from_tunnel_table_subtype_set(
    of_action_ofdpa_set_mpls_tc_from_tunnel_table_t *obj,
    uint16_t subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_SET_MPLS_TC_FROM_TUNNEL_TABLE);
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
    of_wire_buffer_u16_set(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get qos_index from an object of type of_action_ofdpa_set_mpls_tc_from_tunnel_table.
 * @param obj Pointer to an object of type of_action_ofdpa_set_mpls_tc_from_tunnel_table.
 * @param qos_index Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
of_action_ofdpa_set_mpls_tc_from_tunnel_table_qos_index_get(
    of_action_ofdpa_set_mpls_tc_from_tunnel_table_t *obj,
    uint8_t *qos_index)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_SET_MPLS_TC_FROM_TUNNEL_TABLE);
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
    of_wire_buffer_u8_get(wbuf, abs_offset, qos_index);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set qos_index in an object of type of_action_ofdpa_set_mpls_tc_from_tunnel_table.
 * @param obj Pointer to an object of type of_action_ofdpa_set_mpls_tc_from_tunnel_table.
 * @param qos_index The value to write into the object
 */
void
of_action_ofdpa_set_mpls_tc_from_tunnel_table_qos_index_set(
    of_action_ofdpa_set_mpls_tc_from_tunnel_table_t *obj,
    uint8_t qos_index)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_SET_MPLS_TC_FROM_TUNNEL_TABLE);
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
    of_wire_buffer_u8_set(wbuf, abs_offset, qos_index);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get traffic_class from an object of type of_action_ofdpa_set_mpls_tc_from_tunnel_table.
 * @param obj Pointer to an object of type of_action_ofdpa_set_mpls_tc_from_tunnel_table.
 * @param traffic_class Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
of_action_ofdpa_set_mpls_tc_from_tunnel_table_traffic_class_get(
    of_action_ofdpa_set_mpls_tc_from_tunnel_table_t *obj,
    uint8_t *traffic_class)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_SET_MPLS_TC_FROM_TUNNEL_TABLE);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 11;
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
 * Set traffic_class in an object of type of_action_ofdpa_set_mpls_tc_from_tunnel_table.
 * @param obj Pointer to an object of type of_action_ofdpa_set_mpls_tc_from_tunnel_table.
 * @param traffic_class The value to write into the object
 */
void
of_action_ofdpa_set_mpls_tc_from_tunnel_table_traffic_class_set(
    of_action_ofdpa_set_mpls_tc_from_tunnel_table_t *obj,
    uint8_t traffic_class)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_SET_MPLS_TC_FROM_TUNNEL_TABLE);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 11;
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
 * Get color from an object of type of_action_ofdpa_set_mpls_tc_from_tunnel_table.
 * @param obj Pointer to an object of type of_action_ofdpa_set_mpls_tc_from_tunnel_table.
 * @param color Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
of_action_ofdpa_set_mpls_tc_from_tunnel_table_color_get(
    of_action_ofdpa_set_mpls_tc_from_tunnel_table_t *obj,
    uint8_t *color)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_SET_MPLS_TC_FROM_TUNNEL_TABLE);
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
    of_wire_buffer_u8_get(wbuf, abs_offset, color);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set color in an object of type of_action_ofdpa_set_mpls_tc_from_tunnel_table.
 * @param obj Pointer to an object of type of_action_ofdpa_set_mpls_tc_from_tunnel_table.
 * @param color The value to write into the object
 */
void
of_action_ofdpa_set_mpls_tc_from_tunnel_table_color_set(
    of_action_ofdpa_set_mpls_tc_from_tunnel_table_t *obj,
    uint8_t color)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_SET_MPLS_TC_FROM_TUNNEL_TABLE);
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
    of_wire_buffer_u8_set(wbuf, abs_offset, color);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get mpls_tc from an object of type of_action_ofdpa_set_mpls_tc_from_tunnel_table.
 * @param obj Pointer to an object of type of_action_ofdpa_set_mpls_tc_from_tunnel_table.
 * @param mpls_tc Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
of_action_ofdpa_set_mpls_tc_from_tunnel_table_mpls_tc_get(
    of_action_ofdpa_set_mpls_tc_from_tunnel_table_t *obj,
    uint8_t *mpls_tc)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_SET_MPLS_TC_FROM_TUNNEL_TABLE);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 13;
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
 * Set mpls_tc in an object of type of_action_ofdpa_set_mpls_tc_from_tunnel_table.
 * @param obj Pointer to an object of type of_action_ofdpa_set_mpls_tc_from_tunnel_table.
 * @param mpls_tc The value to write into the object
 */
void
of_action_ofdpa_set_mpls_tc_from_tunnel_table_mpls_tc_set(
    of_action_ofdpa_set_mpls_tc_from_tunnel_table_t *obj,
    uint8_t mpls_tc)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_SET_MPLS_TC_FROM_TUNNEL_TABLE);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 13;
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
of_action_id_ofdpa_set_mpls_tc_from_tunnel_table_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint16_t *)(buf + 0) = U16_HTON(0xffff); /* type */
        *(uint32_t *)(buf + 4) = U32_HTON(0x1018); /* experimenter */
        *(uint16_t *)(buf + 8) = U16_HTON(0x11); /* subtype */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_action_id_ofdpa_set_mpls_tc_from_tunnel_table of_action_id_ofdpa_set_mpls_tc_from_tunnel_table
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_action_id_ofdpa_set_mpls_tc_from_tunnel_table_push_wire_values(of_action_id_ofdpa_set_mpls_tc_from_tunnel_table_t *obj)
{

    of_action_id_ofdpa_set_mpls_tc_from_tunnel_table_push_wire_types(obj);

    /* TLV obj; set length */
    of_tlv16_wire_length_set((of_object_t *)obj, obj->length);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_action_id_ofdpa_set_mpls_tc_from_tunnel_table object
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
 * \ingroup of_action_id_ofdpa_set_mpls_tc_from_tunnel_table
 */

of_action_id_ofdpa_set_mpls_tc_from_tunnel_table_t *
of_action_id_ofdpa_set_mpls_tc_from_tunnel_table_new(of_version_t version)
{
    of_action_id_ofdpa_set_mpls_tc_from_tunnel_table_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_ACTION_ID_OFDPA_SET_MPLS_TC_FROM_TUNNEL_TABLE] + of_object_extra_len[version][OF_ACTION_ID_OFDPA_SET_MPLS_TC_FROM_TUNNEL_TABLE];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_action_id_ofdpa_set_mpls_tc_from_tunnel_table_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_action_id_ofdpa_set_mpls_tc_from_tunnel_table_init(obj, version, bytes, 0);

    if (of_action_id_ofdpa_set_mpls_tc_from_tunnel_table_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_action_id_ofdpa_set_mpls_tc_from_tunnel_table.
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
of_action_id_ofdpa_set_mpls_tc_from_tunnel_table_init(of_action_id_ofdpa_set_mpls_tc_from_tunnel_table_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_ACTION_ID_OFDPA_SET_MPLS_TC_FROM_TUNNEL_TABLE] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_ACTION_ID_OFDPA_SET_MPLS_TC_FROM_TUNNEL_TABLE] + of_object_extra_len[version][OF_ACTION_ID_OFDPA_SET_MPLS_TC_FROM_TUNNEL_TABLE];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_ACTION_ID_OFDPA_SET_MPLS_TC_FROM_TUNNEL_TABLE;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_action_id_ofdpa_set_mpls_tc_from_tunnel_table_push_wire_types;

    obj->wire_length_set = of_tlv16_wire_length_set;

    obj->wire_length_get = of_tlv16_wire_length_get;

    obj->wire_type_get = of_action_id_wire_object_id_get;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Get experimenter from an object of type of_action_id_ofdpa_set_mpls_tc_from_tunnel_table.
 * @param obj Pointer to an object of type of_action_id_ofdpa_set_mpls_tc_from_tunnel_table.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_action_id_ofdpa_set_mpls_tc_from_tunnel_table_experimenter_get(
    of_action_id_ofdpa_set_mpls_tc_from_tunnel_table_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_ID_OFDPA_SET_MPLS_TC_FROM_TUNNEL_TABLE);
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
    of_wire_buffer_u32_get(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set experimenter in an object of type of_action_id_ofdpa_set_mpls_tc_from_tunnel_table.
 * @param obj Pointer to an object of type of_action_id_ofdpa_set_mpls_tc_from_tunnel_table.
 * @param experimenter The value to write into the object
 */
void
of_action_id_ofdpa_set_mpls_tc_from_tunnel_table_experimenter_set(
    of_action_id_ofdpa_set_mpls_tc_from_tunnel_table_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_ID_OFDPA_SET_MPLS_TC_FROM_TUNNEL_TABLE);
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
    of_wire_buffer_u32_set(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get subtype from an object of type of_action_id_ofdpa_set_mpls_tc_from_tunnel_table.
 * @param obj Pointer to an object of type of_action_id_ofdpa_set_mpls_tc_from_tunnel_table.
 * @param subtype Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_action_id_ofdpa_set_mpls_tc_from_tunnel_table_subtype_get(
    of_action_id_ofdpa_set_mpls_tc_from_tunnel_table_t *obj,
    uint16_t *subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_ID_OFDPA_SET_MPLS_TC_FROM_TUNNEL_TABLE);
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
    of_wire_buffer_u16_get(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set subtype in an object of type of_action_id_ofdpa_set_mpls_tc_from_tunnel_table.
 * @param obj Pointer to an object of type of_action_id_ofdpa_set_mpls_tc_from_tunnel_table.
 * @param subtype The value to write into the object
 */
void
of_action_id_ofdpa_set_mpls_tc_from_tunnel_table_subtype_set(
    of_action_id_ofdpa_set_mpls_tc_from_tunnel_table_t *obj,
    uint16_t subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_ID_OFDPA_SET_MPLS_TC_FROM_TUNNEL_TABLE);
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
    of_wire_buffer_u16_set(wbuf, abs_offset, subtype);

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
of_action_ofdpa_set_mpls_tc_from_vpn_table_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint16_t *)(buf + 0) = U16_HTON(0xffff); /* type */
        *(uint32_t *)(buf + 4) = U32_HTON(0x1018); /* experimenter */
        *(uint16_t *)(buf + 8) = U16_HTON(0x7); /* subtype */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_action_ofdpa_set_mpls_tc_from_vpn_table of_action_ofdpa_set_mpls_tc_from_vpn_table
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_action_ofdpa_set_mpls_tc_from_vpn_table_push_wire_values(of_action_ofdpa_set_mpls_tc_from_vpn_table_t *obj)
{

    of_action_ofdpa_set_mpls_tc_from_vpn_table_push_wire_types(obj);

    /* TLV obj; set length */
    of_tlv16_wire_length_set((of_object_t *)obj, obj->length);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_action_ofdpa_set_mpls_tc_from_vpn_table object
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
 * \ingroup of_action_ofdpa_set_mpls_tc_from_vpn_table
 */

of_action_ofdpa_set_mpls_tc_from_vpn_table_t *
of_action_ofdpa_set_mpls_tc_from_vpn_table_new(of_version_t version)
{
    of_action_ofdpa_set_mpls_tc_from_vpn_table_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_ACTION_OFDPA_SET_MPLS_TC_FROM_VPN_TABLE] + of_object_extra_len[version][OF_ACTION_OFDPA_SET_MPLS_TC_FROM_VPN_TABLE];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_action_ofdpa_set_mpls_tc_from_vpn_table_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_action_ofdpa_set_mpls_tc_from_vpn_table_init(obj, version, bytes, 0);

    if (of_action_ofdpa_set_mpls_tc_from_vpn_table_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_action_ofdpa_set_mpls_tc_from_vpn_table.
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
of_action_ofdpa_set_mpls_tc_from_vpn_table_init(of_action_ofdpa_set_mpls_tc_from_vpn_table_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_ACTION_OFDPA_SET_MPLS_TC_FROM_VPN_TABLE] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_ACTION_OFDPA_SET_MPLS_TC_FROM_VPN_TABLE] + of_object_extra_len[version][OF_ACTION_OFDPA_SET_MPLS_TC_FROM_VPN_TABLE];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_ACTION_OFDPA_SET_MPLS_TC_FROM_VPN_TABLE;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_action_ofdpa_set_mpls_tc_from_vpn_table_push_wire_types;

    obj->wire_length_set = of_tlv16_wire_length_set;

    obj->wire_length_get = of_tlv16_wire_length_get;

    obj->wire_type_get = of_action_wire_object_id_get;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Get experimenter from an object of type of_action_ofdpa_set_mpls_tc_from_vpn_table.
 * @param obj Pointer to an object of type of_action_ofdpa_set_mpls_tc_from_vpn_table.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_action_ofdpa_set_mpls_tc_from_vpn_table_experimenter_get(
    of_action_ofdpa_set_mpls_tc_from_vpn_table_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_SET_MPLS_TC_FROM_VPN_TABLE);
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
    of_wire_buffer_u32_get(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set experimenter in an object of type of_action_ofdpa_set_mpls_tc_from_vpn_table.
 * @param obj Pointer to an object of type of_action_ofdpa_set_mpls_tc_from_vpn_table.
 * @param experimenter The value to write into the object
 */
void
of_action_ofdpa_set_mpls_tc_from_vpn_table_experimenter_set(
    of_action_ofdpa_set_mpls_tc_from_vpn_table_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_SET_MPLS_TC_FROM_VPN_TABLE);
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
    of_wire_buffer_u32_set(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get subtype from an object of type of_action_ofdpa_set_mpls_tc_from_vpn_table.
 * @param obj Pointer to an object of type of_action_ofdpa_set_mpls_tc_from_vpn_table.
 * @param subtype Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_action_ofdpa_set_mpls_tc_from_vpn_table_subtype_get(
    of_action_ofdpa_set_mpls_tc_from_vpn_table_t *obj,
    uint16_t *subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_SET_MPLS_TC_FROM_VPN_TABLE);
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
    of_wire_buffer_u16_get(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set subtype in an object of type of_action_ofdpa_set_mpls_tc_from_vpn_table.
 * @param obj Pointer to an object of type of_action_ofdpa_set_mpls_tc_from_vpn_table.
 * @param subtype The value to write into the object
 */
void
of_action_ofdpa_set_mpls_tc_from_vpn_table_subtype_set(
    of_action_ofdpa_set_mpls_tc_from_vpn_table_t *obj,
    uint16_t subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_SET_MPLS_TC_FROM_VPN_TABLE);
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
    of_wire_buffer_u16_set(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get qos_index from an object of type of_action_ofdpa_set_mpls_tc_from_vpn_table.
 * @param obj Pointer to an object of type of_action_ofdpa_set_mpls_tc_from_vpn_table.
 * @param qos_index Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
of_action_ofdpa_set_mpls_tc_from_vpn_table_qos_index_get(
    of_action_ofdpa_set_mpls_tc_from_vpn_table_t *obj,
    uint8_t *qos_index)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_SET_MPLS_TC_FROM_VPN_TABLE);
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
    of_wire_buffer_u8_get(wbuf, abs_offset, qos_index);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set qos_index in an object of type of_action_ofdpa_set_mpls_tc_from_vpn_table.
 * @param obj Pointer to an object of type of_action_ofdpa_set_mpls_tc_from_vpn_table.
 * @param qos_index The value to write into the object
 */
void
of_action_ofdpa_set_mpls_tc_from_vpn_table_qos_index_set(
    of_action_ofdpa_set_mpls_tc_from_vpn_table_t *obj,
    uint8_t qos_index)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_SET_MPLS_TC_FROM_VPN_TABLE);
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
    of_wire_buffer_u8_set(wbuf, abs_offset, qos_index);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get traffic_class from an object of type of_action_ofdpa_set_mpls_tc_from_vpn_table.
 * @param obj Pointer to an object of type of_action_ofdpa_set_mpls_tc_from_vpn_table.
 * @param traffic_class Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
of_action_ofdpa_set_mpls_tc_from_vpn_table_traffic_class_get(
    of_action_ofdpa_set_mpls_tc_from_vpn_table_t *obj,
    uint8_t *traffic_class)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_SET_MPLS_TC_FROM_VPN_TABLE);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 11;
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
 * Set traffic_class in an object of type of_action_ofdpa_set_mpls_tc_from_vpn_table.
 * @param obj Pointer to an object of type of_action_ofdpa_set_mpls_tc_from_vpn_table.
 * @param traffic_class The value to write into the object
 */
void
of_action_ofdpa_set_mpls_tc_from_vpn_table_traffic_class_set(
    of_action_ofdpa_set_mpls_tc_from_vpn_table_t *obj,
    uint8_t traffic_class)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_SET_MPLS_TC_FROM_VPN_TABLE);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 11;
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
 * Get color from an object of type of_action_ofdpa_set_mpls_tc_from_vpn_table.
 * @param obj Pointer to an object of type of_action_ofdpa_set_mpls_tc_from_vpn_table.
 * @param color Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
of_action_ofdpa_set_mpls_tc_from_vpn_table_color_get(
    of_action_ofdpa_set_mpls_tc_from_vpn_table_t *obj,
    uint8_t *color)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_SET_MPLS_TC_FROM_VPN_TABLE);
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
    of_wire_buffer_u8_get(wbuf, abs_offset, color);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set color in an object of type of_action_ofdpa_set_mpls_tc_from_vpn_table.
 * @param obj Pointer to an object of type of_action_ofdpa_set_mpls_tc_from_vpn_table.
 * @param color The value to write into the object
 */
void
of_action_ofdpa_set_mpls_tc_from_vpn_table_color_set(
    of_action_ofdpa_set_mpls_tc_from_vpn_table_t *obj,
    uint8_t color)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_SET_MPLS_TC_FROM_VPN_TABLE);
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
    of_wire_buffer_u8_set(wbuf, abs_offset, color);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get mpls_tc from an object of type of_action_ofdpa_set_mpls_tc_from_vpn_table.
 * @param obj Pointer to an object of type of_action_ofdpa_set_mpls_tc_from_vpn_table.
 * @param mpls_tc Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
of_action_ofdpa_set_mpls_tc_from_vpn_table_mpls_tc_get(
    of_action_ofdpa_set_mpls_tc_from_vpn_table_t *obj,
    uint8_t *mpls_tc)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_SET_MPLS_TC_FROM_VPN_TABLE);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 13;
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
 * Set mpls_tc in an object of type of_action_ofdpa_set_mpls_tc_from_vpn_table.
 * @param obj Pointer to an object of type of_action_ofdpa_set_mpls_tc_from_vpn_table.
 * @param mpls_tc The value to write into the object
 */
void
of_action_ofdpa_set_mpls_tc_from_vpn_table_mpls_tc_set(
    of_action_ofdpa_set_mpls_tc_from_vpn_table_t *obj,
    uint8_t mpls_tc)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_SET_MPLS_TC_FROM_VPN_TABLE);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 13;
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
of_action_id_ofdpa_set_mpls_tc_from_vpn_table_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint16_t *)(buf + 0) = U16_HTON(0xffff); /* type */
        *(uint32_t *)(buf + 4) = U32_HTON(0x1018); /* experimenter */
        *(uint16_t *)(buf + 8) = U16_HTON(0x7); /* subtype */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_action_id_ofdpa_set_mpls_tc_from_vpn_table of_action_id_ofdpa_set_mpls_tc_from_vpn_table
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_action_id_ofdpa_set_mpls_tc_from_vpn_table_push_wire_values(of_action_id_ofdpa_set_mpls_tc_from_vpn_table_t *obj)
{

    of_action_id_ofdpa_set_mpls_tc_from_vpn_table_push_wire_types(obj);

    /* TLV obj; set length */
    of_tlv16_wire_length_set((of_object_t *)obj, obj->length);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_action_id_ofdpa_set_mpls_tc_from_vpn_table object
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
 * \ingroup of_action_id_ofdpa_set_mpls_tc_from_vpn_table
 */

of_action_id_ofdpa_set_mpls_tc_from_vpn_table_t *
of_action_id_ofdpa_set_mpls_tc_from_vpn_table_new(of_version_t version)
{
    of_action_id_ofdpa_set_mpls_tc_from_vpn_table_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_ACTION_ID_OFDPA_SET_MPLS_TC_FROM_VPN_TABLE] + of_object_extra_len[version][OF_ACTION_ID_OFDPA_SET_MPLS_TC_FROM_VPN_TABLE];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_action_id_ofdpa_set_mpls_tc_from_vpn_table_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_action_id_ofdpa_set_mpls_tc_from_vpn_table_init(obj, version, bytes, 0);

    if (of_action_id_ofdpa_set_mpls_tc_from_vpn_table_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_action_id_ofdpa_set_mpls_tc_from_vpn_table.
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
of_action_id_ofdpa_set_mpls_tc_from_vpn_table_init(of_action_id_ofdpa_set_mpls_tc_from_vpn_table_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_ACTION_ID_OFDPA_SET_MPLS_TC_FROM_VPN_TABLE] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_ACTION_ID_OFDPA_SET_MPLS_TC_FROM_VPN_TABLE] + of_object_extra_len[version][OF_ACTION_ID_OFDPA_SET_MPLS_TC_FROM_VPN_TABLE];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_ACTION_ID_OFDPA_SET_MPLS_TC_FROM_VPN_TABLE;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_action_id_ofdpa_set_mpls_tc_from_vpn_table_push_wire_types;

    obj->wire_length_set = of_tlv16_wire_length_set;

    obj->wire_length_get = of_tlv16_wire_length_get;

    obj->wire_type_get = of_action_id_wire_object_id_get;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Get experimenter from an object of type of_action_id_ofdpa_set_mpls_tc_from_vpn_table.
 * @param obj Pointer to an object of type of_action_id_ofdpa_set_mpls_tc_from_vpn_table.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_action_id_ofdpa_set_mpls_tc_from_vpn_table_experimenter_get(
    of_action_id_ofdpa_set_mpls_tc_from_vpn_table_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_ID_OFDPA_SET_MPLS_TC_FROM_VPN_TABLE);
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
    of_wire_buffer_u32_get(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set experimenter in an object of type of_action_id_ofdpa_set_mpls_tc_from_vpn_table.
 * @param obj Pointer to an object of type of_action_id_ofdpa_set_mpls_tc_from_vpn_table.
 * @param experimenter The value to write into the object
 */
void
of_action_id_ofdpa_set_mpls_tc_from_vpn_table_experimenter_set(
    of_action_id_ofdpa_set_mpls_tc_from_vpn_table_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_ID_OFDPA_SET_MPLS_TC_FROM_VPN_TABLE);
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
    of_wire_buffer_u32_set(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get subtype from an object of type of_action_id_ofdpa_set_mpls_tc_from_vpn_table.
 * @param obj Pointer to an object of type of_action_id_ofdpa_set_mpls_tc_from_vpn_table.
 * @param subtype Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_action_id_ofdpa_set_mpls_tc_from_vpn_table_subtype_get(
    of_action_id_ofdpa_set_mpls_tc_from_vpn_table_t *obj,
    uint16_t *subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_ID_OFDPA_SET_MPLS_TC_FROM_VPN_TABLE);
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
    of_wire_buffer_u16_get(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set subtype in an object of type of_action_id_ofdpa_set_mpls_tc_from_vpn_table.
 * @param obj Pointer to an object of type of_action_id_ofdpa_set_mpls_tc_from_vpn_table.
 * @param subtype The value to write into the object
 */
void
of_action_id_ofdpa_set_mpls_tc_from_vpn_table_subtype_set(
    of_action_id_ofdpa_set_mpls_tc_from_vpn_table_t *obj,
    uint16_t subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_ID_OFDPA_SET_MPLS_TC_FROM_VPN_TABLE);
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
    of_wire_buffer_u16_set(wbuf, abs_offset, subtype);

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
of_action_ofdpa_set_pcpdei_from_table_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint16_t *)(buf + 0) = U16_HTON(0xffff); /* type */
        *(uint32_t *)(buf + 4) = U32_HTON(0x1018); /* experimenter */
        *(uint16_t *)(buf + 8) = U16_HTON(0x9); /* subtype */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_action_ofdpa_set_pcpdei_from_table of_action_ofdpa_set_pcpdei_from_table
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_action_ofdpa_set_pcpdei_from_table_push_wire_values(of_action_ofdpa_set_pcpdei_from_table_t *obj)
{

    of_action_ofdpa_set_pcpdei_from_table_push_wire_types(obj);

    /* TLV obj; set length */
    of_tlv16_wire_length_set((of_object_t *)obj, obj->length);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_action_ofdpa_set_pcpdei_from_table object
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
 * \ingroup of_action_ofdpa_set_pcpdei_from_table
 */

of_action_ofdpa_set_pcpdei_from_table_t *
of_action_ofdpa_set_pcpdei_from_table_new(of_version_t version)
{
    of_action_ofdpa_set_pcpdei_from_table_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_ACTION_OFDPA_SET_PCPDEI_FROM_TABLE] + of_object_extra_len[version][OF_ACTION_OFDPA_SET_PCPDEI_FROM_TABLE];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_action_ofdpa_set_pcpdei_from_table_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_action_ofdpa_set_pcpdei_from_table_init(obj, version, bytes, 0);

    if (of_action_ofdpa_set_pcpdei_from_table_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_action_ofdpa_set_pcpdei_from_table.
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
of_action_ofdpa_set_pcpdei_from_table_init(of_action_ofdpa_set_pcpdei_from_table_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_ACTION_OFDPA_SET_PCPDEI_FROM_TABLE] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_ACTION_OFDPA_SET_PCPDEI_FROM_TABLE] + of_object_extra_len[version][OF_ACTION_OFDPA_SET_PCPDEI_FROM_TABLE];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_ACTION_OFDPA_SET_PCPDEI_FROM_TABLE;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_action_ofdpa_set_pcpdei_from_table_push_wire_types;

    obj->wire_length_set = of_tlv16_wire_length_set;

    obj->wire_length_get = of_tlv16_wire_length_get;

    obj->wire_type_get = of_action_wire_object_id_get;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Get experimenter from an object of type of_action_ofdpa_set_pcpdei_from_table.
 * @param obj Pointer to an object of type of_action_ofdpa_set_pcpdei_from_table.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_action_ofdpa_set_pcpdei_from_table_experimenter_get(
    of_action_ofdpa_set_pcpdei_from_table_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_SET_PCPDEI_FROM_TABLE);
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
    of_wire_buffer_u32_get(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set experimenter in an object of type of_action_ofdpa_set_pcpdei_from_table.
 * @param obj Pointer to an object of type of_action_ofdpa_set_pcpdei_from_table.
 * @param experimenter The value to write into the object
 */
void
of_action_ofdpa_set_pcpdei_from_table_experimenter_set(
    of_action_ofdpa_set_pcpdei_from_table_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_SET_PCPDEI_FROM_TABLE);
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
    of_wire_buffer_u32_set(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get subtype from an object of type of_action_ofdpa_set_pcpdei_from_table.
 * @param obj Pointer to an object of type of_action_ofdpa_set_pcpdei_from_table.
 * @param subtype Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_action_ofdpa_set_pcpdei_from_table_subtype_get(
    of_action_ofdpa_set_pcpdei_from_table_t *obj,
    uint16_t *subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_SET_PCPDEI_FROM_TABLE);
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
    of_wire_buffer_u16_get(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set subtype in an object of type of_action_ofdpa_set_pcpdei_from_table.
 * @param obj Pointer to an object of type of_action_ofdpa_set_pcpdei_from_table.
 * @param subtype The value to write into the object
 */
void
of_action_ofdpa_set_pcpdei_from_table_subtype_set(
    of_action_ofdpa_set_pcpdei_from_table_t *obj,
    uint16_t subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_SET_PCPDEI_FROM_TABLE);
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
    of_wire_buffer_u16_set(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get qos_index from an object of type of_action_ofdpa_set_pcpdei_from_table.
 * @param obj Pointer to an object of type of_action_ofdpa_set_pcpdei_from_table.
 * @param qos_index Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
of_action_ofdpa_set_pcpdei_from_table_qos_index_get(
    of_action_ofdpa_set_pcpdei_from_table_t *obj,
    uint8_t *qos_index)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_SET_PCPDEI_FROM_TABLE);
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
    of_wire_buffer_u8_get(wbuf, abs_offset, qos_index);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set qos_index in an object of type of_action_ofdpa_set_pcpdei_from_table.
 * @param obj Pointer to an object of type of_action_ofdpa_set_pcpdei_from_table.
 * @param qos_index The value to write into the object
 */
void
of_action_ofdpa_set_pcpdei_from_table_qos_index_set(
    of_action_ofdpa_set_pcpdei_from_table_t *obj,
    uint8_t qos_index)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_SET_PCPDEI_FROM_TABLE);
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
    of_wire_buffer_u8_set(wbuf, abs_offset, qos_index);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get traffic_class from an object of type of_action_ofdpa_set_pcpdei_from_table.
 * @param obj Pointer to an object of type of_action_ofdpa_set_pcpdei_from_table.
 * @param traffic_class Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
of_action_ofdpa_set_pcpdei_from_table_traffic_class_get(
    of_action_ofdpa_set_pcpdei_from_table_t *obj,
    uint8_t *traffic_class)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_SET_PCPDEI_FROM_TABLE);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 11;
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
 * Set traffic_class in an object of type of_action_ofdpa_set_pcpdei_from_table.
 * @param obj Pointer to an object of type of_action_ofdpa_set_pcpdei_from_table.
 * @param traffic_class The value to write into the object
 */
void
of_action_ofdpa_set_pcpdei_from_table_traffic_class_set(
    of_action_ofdpa_set_pcpdei_from_table_t *obj,
    uint8_t traffic_class)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_SET_PCPDEI_FROM_TABLE);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 11;
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
 * Get color from an object of type of_action_ofdpa_set_pcpdei_from_table.
 * @param obj Pointer to an object of type of_action_ofdpa_set_pcpdei_from_table.
 * @param color Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
of_action_ofdpa_set_pcpdei_from_table_color_get(
    of_action_ofdpa_set_pcpdei_from_table_t *obj,
    uint8_t *color)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_SET_PCPDEI_FROM_TABLE);
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
    of_wire_buffer_u8_get(wbuf, abs_offset, color);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set color in an object of type of_action_ofdpa_set_pcpdei_from_table.
 * @param obj Pointer to an object of type of_action_ofdpa_set_pcpdei_from_table.
 * @param color The value to write into the object
 */
void
of_action_ofdpa_set_pcpdei_from_table_color_set(
    of_action_ofdpa_set_pcpdei_from_table_t *obj,
    uint8_t color)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_SET_PCPDEI_FROM_TABLE);
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
of_action_id_ofdpa_set_pcpdei_from_table_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint16_t *)(buf + 0) = U16_HTON(0xffff); /* type */
        *(uint32_t *)(buf + 4) = U32_HTON(0x1018); /* experimenter */
        *(uint16_t *)(buf + 8) = U16_HTON(0x9); /* subtype */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_action_id_ofdpa_set_pcpdei_from_table of_action_id_ofdpa_set_pcpdei_from_table
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_action_id_ofdpa_set_pcpdei_from_table_push_wire_values(of_action_id_ofdpa_set_pcpdei_from_table_t *obj)
{

    of_action_id_ofdpa_set_pcpdei_from_table_push_wire_types(obj);

    /* TLV obj; set length */
    of_tlv16_wire_length_set((of_object_t *)obj, obj->length);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_action_id_ofdpa_set_pcpdei_from_table object
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
 * \ingroup of_action_id_ofdpa_set_pcpdei_from_table
 */

of_action_id_ofdpa_set_pcpdei_from_table_t *
of_action_id_ofdpa_set_pcpdei_from_table_new(of_version_t version)
{
    of_action_id_ofdpa_set_pcpdei_from_table_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_ACTION_ID_OFDPA_SET_PCPDEI_FROM_TABLE] + of_object_extra_len[version][OF_ACTION_ID_OFDPA_SET_PCPDEI_FROM_TABLE];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_action_id_ofdpa_set_pcpdei_from_table_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_action_id_ofdpa_set_pcpdei_from_table_init(obj, version, bytes, 0);

    if (of_action_id_ofdpa_set_pcpdei_from_table_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_action_id_ofdpa_set_pcpdei_from_table.
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
of_action_id_ofdpa_set_pcpdei_from_table_init(of_action_id_ofdpa_set_pcpdei_from_table_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_ACTION_ID_OFDPA_SET_PCPDEI_FROM_TABLE] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_ACTION_ID_OFDPA_SET_PCPDEI_FROM_TABLE] + of_object_extra_len[version][OF_ACTION_ID_OFDPA_SET_PCPDEI_FROM_TABLE];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_ACTION_ID_OFDPA_SET_PCPDEI_FROM_TABLE;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_action_id_ofdpa_set_pcpdei_from_table_push_wire_types;

    obj->wire_length_set = of_tlv16_wire_length_set;

    obj->wire_length_get = of_tlv16_wire_length_get;

    obj->wire_type_get = of_action_id_wire_object_id_get;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Get experimenter from an object of type of_action_id_ofdpa_set_pcpdei_from_table.
 * @param obj Pointer to an object of type of_action_id_ofdpa_set_pcpdei_from_table.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_action_id_ofdpa_set_pcpdei_from_table_experimenter_get(
    of_action_id_ofdpa_set_pcpdei_from_table_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_ID_OFDPA_SET_PCPDEI_FROM_TABLE);
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
    of_wire_buffer_u32_get(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set experimenter in an object of type of_action_id_ofdpa_set_pcpdei_from_table.
 * @param obj Pointer to an object of type of_action_id_ofdpa_set_pcpdei_from_table.
 * @param experimenter The value to write into the object
 */
void
of_action_id_ofdpa_set_pcpdei_from_table_experimenter_set(
    of_action_id_ofdpa_set_pcpdei_from_table_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_ID_OFDPA_SET_PCPDEI_FROM_TABLE);
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
    of_wire_buffer_u32_set(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get subtype from an object of type of_action_id_ofdpa_set_pcpdei_from_table.
 * @param obj Pointer to an object of type of_action_id_ofdpa_set_pcpdei_from_table.
 * @param subtype Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_action_id_ofdpa_set_pcpdei_from_table_subtype_get(
    of_action_id_ofdpa_set_pcpdei_from_table_t *obj,
    uint16_t *subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_ID_OFDPA_SET_PCPDEI_FROM_TABLE);
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
    of_wire_buffer_u16_get(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set subtype in an object of type of_action_id_ofdpa_set_pcpdei_from_table.
 * @param obj Pointer to an object of type of_action_id_ofdpa_set_pcpdei_from_table.
 * @param subtype The value to write into the object
 */
void
of_action_id_ofdpa_set_pcpdei_from_table_subtype_set(
    of_action_id_ofdpa_set_pcpdei_from_table_t *obj,
    uint16_t subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_ID_OFDPA_SET_PCPDEI_FROM_TABLE);
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
    of_wire_buffer_u16_set(wbuf, abs_offset, subtype);

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
of_action_id_output_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint16_t *)(buf + 0) = U16_HTON(0x0); /* type */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_action_id_output of_action_id_output
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_action_id_output_push_wire_values(of_action_id_output_t *obj)
{

    of_action_id_output_push_wire_types(obj);

    /* TLV obj; set length */
    of_tlv16_wire_length_set((of_object_t *)obj, obj->length);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_action_id_output object
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
 * \ingroup of_action_id_output
 */

of_action_id_output_t *
of_action_id_output_new(of_version_t version)
{
    of_action_id_output_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_ACTION_ID_OUTPUT] + of_object_extra_len[version][OF_ACTION_ID_OUTPUT];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_action_id_output_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_action_id_output_init(obj, version, bytes, 0);

    if (of_action_id_output_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_action_id_output.
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
of_action_id_output_init(of_action_id_output_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_ACTION_ID_OUTPUT] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_ACTION_ID_OUTPUT] + of_object_extra_len[version][OF_ACTION_ID_OUTPUT];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_ACTION_ID_OUTPUT;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_action_id_output_push_wire_types;

    obj->wire_length_set = of_tlv16_wire_length_set;

    obj->wire_length_get = of_tlv16_wire_length_get;

    obj->wire_type_get = of_action_id_wire_object_id_get;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
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
of_action_id_pop_mpls_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint16_t *)(buf + 0) = U16_HTON(0x14); /* type */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_action_id_pop_mpls of_action_id_pop_mpls
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_action_id_pop_mpls_push_wire_values(of_action_id_pop_mpls_t *obj)
{

    of_action_id_pop_mpls_push_wire_types(obj);

    /* TLV obj; set length */
    of_tlv16_wire_length_set((of_object_t *)obj, obj->length);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_action_id_pop_mpls object
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
 * \ingroup of_action_id_pop_mpls
 */

of_action_id_pop_mpls_t *
of_action_id_pop_mpls_new(of_version_t version)
{
    of_action_id_pop_mpls_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_ACTION_ID_POP_MPLS] + of_object_extra_len[version][OF_ACTION_ID_POP_MPLS];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_action_id_pop_mpls_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_action_id_pop_mpls_init(obj, version, bytes, 0);

    if (of_action_id_pop_mpls_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_action_id_pop_mpls.
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
of_action_id_pop_mpls_init(of_action_id_pop_mpls_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_ACTION_ID_POP_MPLS] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_ACTION_ID_POP_MPLS] + of_object_extra_len[version][OF_ACTION_ID_POP_MPLS];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_ACTION_ID_POP_MPLS;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_action_id_pop_mpls_push_wire_types;

    obj->wire_length_set = of_tlv16_wire_length_set;

    obj->wire_length_get = of_tlv16_wire_length_get;

    obj->wire_type_get = of_action_id_wire_object_id_get;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
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
of_action_pop_pbb_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint16_t *)(buf + 0) = U16_HTON(0x1b); /* type */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_action_pop_pbb of_action_pop_pbb
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_action_pop_pbb_push_wire_values(of_action_pop_pbb_t *obj)
{

    of_action_pop_pbb_push_wire_types(obj);

    /* TLV obj; set length */
    of_tlv16_wire_length_set((of_object_t *)obj, obj->length);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_action_pop_pbb object
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
 * \ingroup of_action_pop_pbb
 */

of_action_pop_pbb_t *
of_action_pop_pbb_new(of_version_t version)
{
    of_action_pop_pbb_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_ACTION_POP_PBB] + of_object_extra_len[version][OF_ACTION_POP_PBB];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_action_pop_pbb_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_action_pop_pbb_init(obj, version, bytes, 0);

    if (of_action_pop_pbb_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_action_pop_pbb.
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
of_action_pop_pbb_init(of_action_pop_pbb_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_ACTION_POP_PBB] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_ACTION_POP_PBB] + of_object_extra_len[version][OF_ACTION_POP_PBB];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_ACTION_POP_PBB;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_action_pop_pbb_push_wire_types;

    obj->wire_length_set = of_tlv16_wire_length_set;

    obj->wire_length_get = of_tlv16_wire_length_get;

    obj->wire_type_get = of_action_wire_object_id_get;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
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
of_action_id_pop_pbb_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint16_t *)(buf + 0) = U16_HTON(0x1b); /* type */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_action_id_pop_pbb of_action_id_pop_pbb
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_action_id_pop_pbb_push_wire_values(of_action_id_pop_pbb_t *obj)
{

    of_action_id_pop_pbb_push_wire_types(obj);

    /* TLV obj; set length */
    of_tlv16_wire_length_set((of_object_t *)obj, obj->length);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_action_id_pop_pbb object
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
 * \ingroup of_action_id_pop_pbb
 */

of_action_id_pop_pbb_t *
of_action_id_pop_pbb_new(of_version_t version)
{
    of_action_id_pop_pbb_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_ACTION_ID_POP_PBB] + of_object_extra_len[version][OF_ACTION_ID_POP_PBB];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_action_id_pop_pbb_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_action_id_pop_pbb_init(obj, version, bytes, 0);

    if (of_action_id_pop_pbb_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_action_id_pop_pbb.
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
of_action_id_pop_pbb_init(of_action_id_pop_pbb_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_ACTION_ID_POP_PBB] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_ACTION_ID_POP_PBB] + of_object_extra_len[version][OF_ACTION_ID_POP_PBB];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_ACTION_ID_POP_PBB;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_action_id_pop_pbb_push_wire_types;

    obj->wire_length_set = of_tlv16_wire_length_set;

    obj->wire_length_get = of_tlv16_wire_length_get;

    obj->wire_type_get = of_action_id_wire_object_id_get;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
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
of_action_id_pop_vlan_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint16_t *)(buf + 0) = U16_HTON(0x12); /* type */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_action_id_pop_vlan of_action_id_pop_vlan
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_action_id_pop_vlan_push_wire_values(of_action_id_pop_vlan_t *obj)
{

    of_action_id_pop_vlan_push_wire_types(obj);

    /* TLV obj; set length */
    of_tlv16_wire_length_set((of_object_t *)obj, obj->length);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_action_id_pop_vlan object
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
 * \ingroup of_action_id_pop_vlan
 */

of_action_id_pop_vlan_t *
of_action_id_pop_vlan_new(of_version_t version)
{
    of_action_id_pop_vlan_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_ACTION_ID_POP_VLAN] + of_object_extra_len[version][OF_ACTION_ID_POP_VLAN];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_action_id_pop_vlan_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_action_id_pop_vlan_init(obj, version, bytes, 0);

    if (of_action_id_pop_vlan_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_action_id_pop_vlan.
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
of_action_id_pop_vlan_init(of_action_id_pop_vlan_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_ACTION_ID_POP_VLAN] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_ACTION_ID_POP_VLAN] + of_object_extra_len[version][OF_ACTION_ID_POP_VLAN];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_ACTION_ID_POP_VLAN;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_action_id_pop_vlan_push_wire_types;

    obj->wire_length_set = of_tlv16_wire_length_set;

    obj->wire_length_get = of_tlv16_wire_length_get;

    obj->wire_type_get = of_action_id_wire_object_id_get;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
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
of_action_id_push_mpls_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint16_t *)(buf + 0) = U16_HTON(0x13); /* type */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_action_id_push_mpls of_action_id_push_mpls
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_action_id_push_mpls_push_wire_values(of_action_id_push_mpls_t *obj)
{

    of_action_id_push_mpls_push_wire_types(obj);

    /* TLV obj; set length */
    of_tlv16_wire_length_set((of_object_t *)obj, obj->length);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_action_id_push_mpls object
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
 * \ingroup of_action_id_push_mpls
 */

of_action_id_push_mpls_t *
of_action_id_push_mpls_new(of_version_t version)
{
    of_action_id_push_mpls_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_ACTION_ID_PUSH_MPLS] + of_object_extra_len[version][OF_ACTION_ID_PUSH_MPLS];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_action_id_push_mpls_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_action_id_push_mpls_init(obj, version, bytes, 0);

    if (of_action_id_push_mpls_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_action_id_push_mpls.
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
of_action_id_push_mpls_init(of_action_id_push_mpls_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_ACTION_ID_PUSH_MPLS] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_ACTION_ID_PUSH_MPLS] + of_object_extra_len[version][OF_ACTION_ID_PUSH_MPLS];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_ACTION_ID_PUSH_MPLS;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_action_id_push_mpls_push_wire_types;

    obj->wire_length_set = of_tlv16_wire_length_set;

    obj->wire_length_get = of_tlv16_wire_length_get;

    obj->wire_type_get = of_action_id_wire_object_id_get;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
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
of_action_push_pbb_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint16_t *)(buf + 0) = U16_HTON(0x1a); /* type */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_action_push_pbb of_action_push_pbb
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_action_push_pbb_push_wire_values(of_action_push_pbb_t *obj)
{

    of_action_push_pbb_push_wire_types(obj);

    /* TLV obj; set length */
    of_tlv16_wire_length_set((of_object_t *)obj, obj->length);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_action_push_pbb object
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
 * \ingroup of_action_push_pbb
 */

of_action_push_pbb_t *
of_action_push_pbb_new(of_version_t version)
{
    of_action_push_pbb_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_ACTION_PUSH_PBB] + of_object_extra_len[version][OF_ACTION_PUSH_PBB];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_action_push_pbb_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_action_push_pbb_init(obj, version, bytes, 0);

    if (of_action_push_pbb_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_action_push_pbb.
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
of_action_push_pbb_init(of_action_push_pbb_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_ACTION_PUSH_PBB] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_ACTION_PUSH_PBB] + of_object_extra_len[version][OF_ACTION_PUSH_PBB];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_ACTION_PUSH_PBB;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_action_push_pbb_push_wire_types;

    obj->wire_length_set = of_tlv16_wire_length_set;

    obj->wire_length_get = of_tlv16_wire_length_get;

    obj->wire_type_get = of_action_wire_object_id_get;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Get ethertype from an object of type of_action_push_pbb.
 * @param obj Pointer to an object of type of_action_push_pbb.
 * @param ethertype Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_action_push_pbb_ethertype_get(
    of_action_push_pbb_t *obj,
    uint16_t *ethertype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_PUSH_PBB);
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
    of_wire_buffer_u16_get(wbuf, abs_offset, ethertype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set ethertype in an object of type of_action_push_pbb.
 * @param obj Pointer to an object of type of_action_push_pbb.
 * @param ethertype The value to write into the object
 */
void
of_action_push_pbb_ethertype_set(
    of_action_push_pbb_t *obj,
    uint16_t ethertype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_PUSH_PBB);
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
    of_wire_buffer_u16_set(wbuf, abs_offset, ethertype);

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
of_action_id_push_pbb_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint16_t *)(buf + 0) = U16_HTON(0x1a); /* type */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_action_id_push_pbb of_action_id_push_pbb
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_action_id_push_pbb_push_wire_values(of_action_id_push_pbb_t *obj)
{

    of_action_id_push_pbb_push_wire_types(obj);

    /* TLV obj; set length */
    of_tlv16_wire_length_set((of_object_t *)obj, obj->length);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_action_id_push_pbb object
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
 * \ingroup of_action_id_push_pbb
 */

of_action_id_push_pbb_t *
of_action_id_push_pbb_new(of_version_t version)
{
    of_action_id_push_pbb_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_ACTION_ID_PUSH_PBB] + of_object_extra_len[version][OF_ACTION_ID_PUSH_PBB];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_action_id_push_pbb_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_action_id_push_pbb_init(obj, version, bytes, 0);

    if (of_action_id_push_pbb_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_action_id_push_pbb.
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
of_action_id_push_pbb_init(of_action_id_push_pbb_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_ACTION_ID_PUSH_PBB] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_ACTION_ID_PUSH_PBB] + of_object_extra_len[version][OF_ACTION_ID_PUSH_PBB];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_ACTION_ID_PUSH_PBB;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_action_id_push_pbb_push_wire_types;

    obj->wire_length_set = of_tlv16_wire_length_set;

    obj->wire_length_get = of_tlv16_wire_length_get;

    obj->wire_type_get = of_action_id_wire_object_id_get;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
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
of_action_id_push_vlan_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint16_t *)(buf + 0) = U16_HTON(0x11); /* type */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_action_id_push_vlan of_action_id_push_vlan
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_action_id_push_vlan_push_wire_values(of_action_id_push_vlan_t *obj)
{

    of_action_id_push_vlan_push_wire_types(obj);

    /* TLV obj; set length */
    of_tlv16_wire_length_set((of_object_t *)obj, obj->length);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_action_id_push_vlan object
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
 * \ingroup of_action_id_push_vlan
 */

of_action_id_push_vlan_t *
of_action_id_push_vlan_new(of_version_t version)
{
    of_action_id_push_vlan_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_ACTION_ID_PUSH_VLAN] + of_object_extra_len[version][OF_ACTION_ID_PUSH_VLAN];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_action_id_push_vlan_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_action_id_push_vlan_init(obj, version, bytes, 0);

    if (of_action_id_push_vlan_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_action_id_push_vlan.
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
of_action_id_push_vlan_init(of_action_id_push_vlan_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_ACTION_ID_PUSH_VLAN] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_ACTION_ID_PUSH_VLAN] + of_object_extra_len[version][OF_ACTION_ID_PUSH_VLAN];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_ACTION_ID_PUSH_VLAN;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_action_id_push_vlan_push_wire_types;

    obj->wire_length_set = of_tlv16_wire_length_set;

    obj->wire_length_get = of_tlv16_wire_length_get;

    obj->wire_type_get = of_action_id_wire_object_id_get;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
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
of_action_id_set_field_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint16_t *)(buf + 0) = U16_HTON(0x19); /* type */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_action_id_set_field of_action_id_set_field
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_action_id_set_field_push_wire_values(of_action_id_set_field_t *obj)
{

    of_action_id_set_field_push_wire_types(obj);

    /* TLV obj; set length */
    of_tlv16_wire_length_set((of_object_t *)obj, obj->length);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_action_id_set_field object
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
 * \ingroup of_action_id_set_field
 */

of_action_id_set_field_t *
of_action_id_set_field_new(of_version_t version)
{
    of_action_id_set_field_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_ACTION_ID_SET_FIELD] + of_object_extra_len[version][OF_ACTION_ID_SET_FIELD];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_action_id_set_field_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_action_id_set_field_init(obj, version, bytes, 0);

    if (of_action_id_set_field_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_action_id_set_field.
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
of_action_id_set_field_init(of_action_id_set_field_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_ACTION_ID_SET_FIELD] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_ACTION_ID_SET_FIELD] + of_object_extra_len[version][OF_ACTION_ID_SET_FIELD];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_ACTION_ID_SET_FIELD;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_action_id_set_field_push_wire_types;

    obj->wire_length_set = of_tlv16_wire_length_set;

    obj->wire_length_get = of_tlv16_wire_length_get;

    obj->wire_type_get = of_action_id_wire_object_id_get;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
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
of_action_id_set_mpls_ttl_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint16_t *)(buf + 0) = U16_HTON(0xf); /* type */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_action_id_set_mpls_ttl of_action_id_set_mpls_ttl
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_action_id_set_mpls_ttl_push_wire_values(of_action_id_set_mpls_ttl_t *obj)
{

    of_action_id_set_mpls_ttl_push_wire_types(obj);

    /* TLV obj; set length */
    of_tlv16_wire_length_set((of_object_t *)obj, obj->length);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_action_id_set_mpls_ttl object
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
 * \ingroup of_action_id_set_mpls_ttl
 */

of_action_id_set_mpls_ttl_t *
of_action_id_set_mpls_ttl_new(of_version_t version)
{
    of_action_id_set_mpls_ttl_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_ACTION_ID_SET_MPLS_TTL] + of_object_extra_len[version][OF_ACTION_ID_SET_MPLS_TTL];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_action_id_set_mpls_ttl_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_action_id_set_mpls_ttl_init(obj, version, bytes, 0);

    if (of_action_id_set_mpls_ttl_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_action_id_set_mpls_ttl.
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
of_action_id_set_mpls_ttl_init(of_action_id_set_mpls_ttl_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_ACTION_ID_SET_MPLS_TTL] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_ACTION_ID_SET_MPLS_TTL] + of_object_extra_len[version][OF_ACTION_ID_SET_MPLS_TTL];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_ACTION_ID_SET_MPLS_TTL;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_action_id_set_mpls_ttl_push_wire_types;

    obj->wire_length_set = of_tlv16_wire_length_set;

    obj->wire_length_get = of_tlv16_wire_length_get;

    obj->wire_type_get = of_action_id_wire_object_id_get;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
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
of_action_id_set_nw_ttl_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint16_t *)(buf + 0) = U16_HTON(0x17); /* type */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_action_id_set_nw_ttl of_action_id_set_nw_ttl
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_action_id_set_nw_ttl_push_wire_values(of_action_id_set_nw_ttl_t *obj)
{

    of_action_id_set_nw_ttl_push_wire_types(obj);

    /* TLV obj; set length */
    of_tlv16_wire_length_set((of_object_t *)obj, obj->length);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_action_id_set_nw_ttl object
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
 * \ingroup of_action_id_set_nw_ttl
 */

of_action_id_set_nw_ttl_t *
of_action_id_set_nw_ttl_new(of_version_t version)
{
    of_action_id_set_nw_ttl_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_ACTION_ID_SET_NW_TTL] + of_object_extra_len[version][OF_ACTION_ID_SET_NW_TTL];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_action_id_set_nw_ttl_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_action_id_set_nw_ttl_init(obj, version, bytes, 0);

    if (of_action_id_set_nw_ttl_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_action_id_set_nw_ttl.
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
of_action_id_set_nw_ttl_init(of_action_id_set_nw_ttl_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_ACTION_ID_SET_NW_TTL] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_ACTION_ID_SET_NW_TTL] + of_object_extra_len[version][OF_ACTION_ID_SET_NW_TTL];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_ACTION_ID_SET_NW_TTL;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_action_id_set_nw_ttl_push_wire_types;

    obj->wire_length_set = of_tlv16_wire_length_set;

    obj->wire_length_get = of_tlv16_wire_length_get;

    obj->wire_type_get = of_action_id_wire_object_id_get;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
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
of_action_id_set_queue_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint16_t *)(buf + 0) = U16_HTON(0x15); /* type */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_action_id_set_queue of_action_id_set_queue
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_action_id_set_queue_push_wire_values(of_action_id_set_queue_t *obj)
{

    of_action_id_set_queue_push_wire_types(obj);

    /* TLV obj; set length */
    of_tlv16_wire_length_set((of_object_t *)obj, obj->length);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_action_id_set_queue object
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
 * \ingroup of_action_id_set_queue
 */

of_action_id_set_queue_t *
of_action_id_set_queue_new(of_version_t version)
{
    of_action_id_set_queue_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_ACTION_ID_SET_QUEUE] + of_object_extra_len[version][OF_ACTION_ID_SET_QUEUE];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_action_id_set_queue_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_action_id_set_queue_init(obj, version, bytes, 0);

    if (of_action_id_set_queue_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_action_id_set_queue.
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
of_action_id_set_queue_init(of_action_id_set_queue_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_ACTION_ID_SET_QUEUE] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_ACTION_ID_SET_QUEUE] + of_object_extra_len[version][OF_ACTION_ID_SET_QUEUE];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_ACTION_ID_SET_QUEUE;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_action_id_set_queue_push_wire_types;

    obj->wire_length_set = of_tlv16_wire_length_set;

    obj->wire_length_get = of_tlv16_wire_length_get;

    obj->wire_type_get = of_action_id_wire_object_id_get;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
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
of_async_get_reply_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint8_t *)(buf + 0) = obj->version; /* version */
        *(uint8_t *)(buf + 1) = 0x1b; /* type */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_async_get_reply of_async_get_reply
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_async_get_reply_push_wire_values(of_async_get_reply_t *obj)
{

    of_async_get_reply_push_wire_types(obj);

    /* Message obj; set length */
    of_message_t msg;

    if ((msg = OF_OBJECT_TO_MESSAGE(obj)) != NULL) {
        of_message_length_set(msg, obj->length);
    }

    return OF_ERROR_NONE;
}

/**
 * Create a new of_async_get_reply object
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
 * \ingroup of_async_get_reply
 */

of_async_get_reply_t *
of_async_get_reply_new(of_version_t version)
{
    of_async_get_reply_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_ASYNC_GET_REPLY] + of_object_extra_len[version][OF_ASYNC_GET_REPLY];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_async_get_reply_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_async_get_reply_init(obj, version, bytes, 0);

    if (of_async_get_reply_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_async_get_reply.
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
of_async_get_reply_init(of_async_get_reply_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_ASYNC_GET_REPLY] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_ASYNC_GET_REPLY] + of_object_extra_len[version][OF_ASYNC_GET_REPLY];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_ASYNC_GET_REPLY;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_async_get_reply_push_wire_types;

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
 * Create a new of_async_get_reply object and bind it to an existing message
 *
 * @param msg The message to bind the new object to
 * @return Pointer to the newly create object or NULL on error
 *
 * \ingroup of_async_get_reply
 */

of_async_get_reply_t *
of_async_get_reply_new_from_message(of_message_t msg)
{
    of_async_get_reply_t *obj = NULL;
    of_version_t version;
    int length;

    if (msg == NULL) return NULL;

    version = of_message_version_get(msg);
    if (!OF_VERSION_OKAY(version)) return NULL;

    length = of_message_length_get(msg);

    if ((obj = (of_async_get_reply_t *)of_object_new(-1)) == NULL) {
        return NULL;
    }

    of_async_get_reply_init(obj, version, 0, 0);

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
 * Get xid from an object of type of_async_get_reply.
 * @param obj Pointer to an object of type of_async_get_reply.
 * @param xid Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_async_get_reply_xid_get(
    of_async_get_reply_t *obj,
    uint32_t *xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ASYNC_GET_REPLY);
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
 * Set xid in an object of type of_async_get_reply.
 * @param obj Pointer to an object of type of_async_get_reply.
 * @param xid The value to write into the object
 */
void
of_async_get_reply_xid_set(
    of_async_get_reply_t *obj,
    uint32_t xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ASYNC_GET_REPLY);
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
 * Get packet_in_mask_equal_master from an object of type of_async_get_reply.
 * @param obj Pointer to an object of type of_async_get_reply.
 * @param packet_in_mask_equal_master Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_async_get_reply_packet_in_mask_equal_master_get(
    of_async_get_reply_t *obj,
    uint32_t *packet_in_mask_equal_master)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ASYNC_GET_REPLY);
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
    of_wire_buffer_u32_get(wbuf, abs_offset, packet_in_mask_equal_master);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set packet_in_mask_equal_master in an object of type of_async_get_reply.
 * @param obj Pointer to an object of type of_async_get_reply.
 * @param packet_in_mask_equal_master The value to write into the object
 */
void
of_async_get_reply_packet_in_mask_equal_master_set(
    of_async_get_reply_t *obj,
    uint32_t packet_in_mask_equal_master)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ASYNC_GET_REPLY);
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
    of_wire_buffer_u32_set(wbuf, abs_offset, packet_in_mask_equal_master);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get packet_in_mask_slave from an object of type of_async_get_reply.
 * @param obj Pointer to an object of type of_async_get_reply.
 * @param packet_in_mask_slave Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_async_get_reply_packet_in_mask_slave_get(
    of_async_get_reply_t *obj,
    uint32_t *packet_in_mask_slave)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ASYNC_GET_REPLY);
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
    of_wire_buffer_u32_get(wbuf, abs_offset, packet_in_mask_slave);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set packet_in_mask_slave in an object of type of_async_get_reply.
 * @param obj Pointer to an object of type of_async_get_reply.
 * @param packet_in_mask_slave The value to write into the object
 */
void
of_async_get_reply_packet_in_mask_slave_set(
    of_async_get_reply_t *obj,
    uint32_t packet_in_mask_slave)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ASYNC_GET_REPLY);
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
    of_wire_buffer_u32_set(wbuf, abs_offset, packet_in_mask_slave);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get port_status_mask_equal_master from an object of type of_async_get_reply.
 * @param obj Pointer to an object of type of_async_get_reply.
 * @param port_status_mask_equal_master Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_async_get_reply_port_status_mask_equal_master_get(
    of_async_get_reply_t *obj,
    uint32_t *port_status_mask_equal_master)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ASYNC_GET_REPLY);
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
    of_wire_buffer_u32_get(wbuf, abs_offset, port_status_mask_equal_master);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set port_status_mask_equal_master in an object of type of_async_get_reply.
 * @param obj Pointer to an object of type of_async_get_reply.
 * @param port_status_mask_equal_master The value to write into the object
 */
void
of_async_get_reply_port_status_mask_equal_master_set(
    of_async_get_reply_t *obj,
    uint32_t port_status_mask_equal_master)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ASYNC_GET_REPLY);
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
    of_wire_buffer_u32_set(wbuf, abs_offset, port_status_mask_equal_master);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get port_status_mask_slave from an object of type of_async_get_reply.
 * @param obj Pointer to an object of type of_async_get_reply.
 * @param port_status_mask_slave Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_async_get_reply_port_status_mask_slave_get(
    of_async_get_reply_t *obj,
    uint32_t *port_status_mask_slave)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ASYNC_GET_REPLY);
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
    of_wire_buffer_u32_get(wbuf, abs_offset, port_status_mask_slave);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set port_status_mask_slave in an object of type of_async_get_reply.
 * @param obj Pointer to an object of type of_async_get_reply.
 * @param port_status_mask_slave The value to write into the object
 */
void
of_async_get_reply_port_status_mask_slave_set(
    of_async_get_reply_t *obj,
    uint32_t port_status_mask_slave)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ASYNC_GET_REPLY);
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
    of_wire_buffer_u32_set(wbuf, abs_offset, port_status_mask_slave);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get flow_removed_mask_equal_master from an object of type of_async_get_reply.
 * @param obj Pointer to an object of type of_async_get_reply.
 * @param flow_removed_mask_equal_master Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_async_get_reply_flow_removed_mask_equal_master_get(
    of_async_get_reply_t *obj,
    uint32_t *flow_removed_mask_equal_master)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ASYNC_GET_REPLY);
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
    of_wire_buffer_u32_get(wbuf, abs_offset, flow_removed_mask_equal_master);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set flow_removed_mask_equal_master in an object of type of_async_get_reply.
 * @param obj Pointer to an object of type of_async_get_reply.
 * @param flow_removed_mask_equal_master The value to write into the object
 */
void
of_async_get_reply_flow_removed_mask_equal_master_set(
    of_async_get_reply_t *obj,
    uint32_t flow_removed_mask_equal_master)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ASYNC_GET_REPLY);
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
    of_wire_buffer_u32_set(wbuf, abs_offset, flow_removed_mask_equal_master);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get flow_removed_mask_slave from an object of type of_async_get_reply.
 * @param obj Pointer to an object of type of_async_get_reply.
 * @param flow_removed_mask_slave Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_async_get_reply_flow_removed_mask_slave_get(
    of_async_get_reply_t *obj,
    uint32_t *flow_removed_mask_slave)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ASYNC_GET_REPLY);
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
    of_wire_buffer_u32_get(wbuf, abs_offset, flow_removed_mask_slave);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set flow_removed_mask_slave in an object of type of_async_get_reply.
 * @param obj Pointer to an object of type of_async_get_reply.
 * @param flow_removed_mask_slave The value to write into the object
 */
void
of_async_get_reply_flow_removed_mask_slave_set(
    of_async_get_reply_t *obj,
    uint32_t flow_removed_mask_slave)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ASYNC_GET_REPLY);
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
    of_wire_buffer_u32_set(wbuf, abs_offset, flow_removed_mask_slave);

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
of_async_get_request_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint8_t *)(buf + 0) = obj->version; /* version */
        *(uint8_t *)(buf + 1) = 0x1a; /* type */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_async_get_request of_async_get_request
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_async_get_request_push_wire_values(of_async_get_request_t *obj)
{

    of_async_get_request_push_wire_types(obj);

    /* Message obj; set length */
    of_message_t msg;

    if ((msg = OF_OBJECT_TO_MESSAGE(obj)) != NULL) {
        of_message_length_set(msg, obj->length);
    }

    return OF_ERROR_NONE;
}

/**
 * Create a new of_async_get_request object
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
 * \ingroup of_async_get_request
 */

of_async_get_request_t *
of_async_get_request_new(of_version_t version)
{
    of_async_get_request_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_ASYNC_GET_REQUEST] + of_object_extra_len[version][OF_ASYNC_GET_REQUEST];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_async_get_request_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_async_get_request_init(obj, version, bytes, 0);

    if (of_async_get_request_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_async_get_request.
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
of_async_get_request_init(of_async_get_request_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_ASYNC_GET_REQUEST] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_ASYNC_GET_REQUEST] + of_object_extra_len[version][OF_ASYNC_GET_REQUEST];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_ASYNC_GET_REQUEST;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_async_get_request_push_wire_types;

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
 * Create a new of_async_get_request object and bind it to an existing message
 *
 * @param msg The message to bind the new object to
 * @return Pointer to the newly create object or NULL on error
 *
 * \ingroup of_async_get_request
 */

of_async_get_request_t *
of_async_get_request_new_from_message(of_message_t msg)
{
    of_async_get_request_t *obj = NULL;
    of_version_t version;
    int length;

    if (msg == NULL) return NULL;

    version = of_message_version_get(msg);
    if (!OF_VERSION_OKAY(version)) return NULL;

    length = of_message_length_get(msg);

    if ((obj = (of_async_get_request_t *)of_object_new(-1)) == NULL) {
        return NULL;
    }

    of_async_get_request_init(obj, version, 0, 0);

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
 * Get xid from an object of type of_async_get_request.
 * @param obj Pointer to an object of type of_async_get_request.
 * @param xid Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_async_get_request_xid_get(
    of_async_get_request_t *obj,
    uint32_t *xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ASYNC_GET_REQUEST);
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
 * Set xid in an object of type of_async_get_request.
 * @param obj Pointer to an object of type of_async_get_request.
 * @param xid The value to write into the object
 */
void
of_async_get_request_xid_set(
    of_async_get_request_t *obj,
    uint32_t xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ASYNC_GET_REQUEST);
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
 * Get packet_in_mask_equal_master from an object of type of_async_get_request.
 * @param obj Pointer to an object of type of_async_get_request.
 * @param packet_in_mask_equal_master Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_async_get_request_packet_in_mask_equal_master_get(
    of_async_get_request_t *obj,
    uint32_t *packet_in_mask_equal_master)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ASYNC_GET_REQUEST);
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
    of_wire_buffer_u32_get(wbuf, abs_offset, packet_in_mask_equal_master);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set packet_in_mask_equal_master in an object of type of_async_get_request.
 * @param obj Pointer to an object of type of_async_get_request.
 * @param packet_in_mask_equal_master The value to write into the object
 */
void
of_async_get_request_packet_in_mask_equal_master_set(
    of_async_get_request_t *obj,
    uint32_t packet_in_mask_equal_master)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ASYNC_GET_REQUEST);
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
    of_wire_buffer_u32_set(wbuf, abs_offset, packet_in_mask_equal_master);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get packet_in_mask_slave from an object of type of_async_get_request.
 * @param obj Pointer to an object of type of_async_get_request.
 * @param packet_in_mask_slave Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_async_get_request_packet_in_mask_slave_get(
    of_async_get_request_t *obj,
    uint32_t *packet_in_mask_slave)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ASYNC_GET_REQUEST);
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
    of_wire_buffer_u32_get(wbuf, abs_offset, packet_in_mask_slave);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set packet_in_mask_slave in an object of type of_async_get_request.
 * @param obj Pointer to an object of type of_async_get_request.
 * @param packet_in_mask_slave The value to write into the object
 */
void
of_async_get_request_packet_in_mask_slave_set(
    of_async_get_request_t *obj,
    uint32_t packet_in_mask_slave)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ASYNC_GET_REQUEST);
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
    of_wire_buffer_u32_set(wbuf, abs_offset, packet_in_mask_slave);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get port_status_mask_equal_master from an object of type of_async_get_request.
 * @param obj Pointer to an object of type of_async_get_request.
 * @param port_status_mask_equal_master Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_async_get_request_port_status_mask_equal_master_get(
    of_async_get_request_t *obj,
    uint32_t *port_status_mask_equal_master)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ASYNC_GET_REQUEST);
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
    of_wire_buffer_u32_get(wbuf, abs_offset, port_status_mask_equal_master);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set port_status_mask_equal_master in an object of type of_async_get_request.
 * @param obj Pointer to an object of type of_async_get_request.
 * @param port_status_mask_equal_master The value to write into the object
 */
void
of_async_get_request_port_status_mask_equal_master_set(
    of_async_get_request_t *obj,
    uint32_t port_status_mask_equal_master)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ASYNC_GET_REQUEST);
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
    of_wire_buffer_u32_set(wbuf, abs_offset, port_status_mask_equal_master);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get port_status_mask_slave from an object of type of_async_get_request.
 * @param obj Pointer to an object of type of_async_get_request.
 * @param port_status_mask_slave Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_async_get_request_port_status_mask_slave_get(
    of_async_get_request_t *obj,
    uint32_t *port_status_mask_slave)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ASYNC_GET_REQUEST);
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
    of_wire_buffer_u32_get(wbuf, abs_offset, port_status_mask_slave);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set port_status_mask_slave in an object of type of_async_get_request.
 * @param obj Pointer to an object of type of_async_get_request.
 * @param port_status_mask_slave The value to write into the object
 */
void
of_async_get_request_port_status_mask_slave_set(
    of_async_get_request_t *obj,
    uint32_t port_status_mask_slave)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ASYNC_GET_REQUEST);
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
    of_wire_buffer_u32_set(wbuf, abs_offset, port_status_mask_slave);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get flow_removed_mask_equal_master from an object of type of_async_get_request.
 * @param obj Pointer to an object of type of_async_get_request.
 * @param flow_removed_mask_equal_master Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_async_get_request_flow_removed_mask_equal_master_get(
    of_async_get_request_t *obj,
    uint32_t *flow_removed_mask_equal_master)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ASYNC_GET_REQUEST);
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
    of_wire_buffer_u32_get(wbuf, abs_offset, flow_removed_mask_equal_master);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set flow_removed_mask_equal_master in an object of type of_async_get_request.
 * @param obj Pointer to an object of type of_async_get_request.
 * @param flow_removed_mask_equal_master The value to write into the object
 */
void
of_async_get_request_flow_removed_mask_equal_master_set(
    of_async_get_request_t *obj,
    uint32_t flow_removed_mask_equal_master)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ASYNC_GET_REQUEST);
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
    of_wire_buffer_u32_set(wbuf, abs_offset, flow_removed_mask_equal_master);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get flow_removed_mask_slave from an object of type of_async_get_request.
 * @param obj Pointer to an object of type of_async_get_request.
 * @param flow_removed_mask_slave Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_async_get_request_flow_removed_mask_slave_get(
    of_async_get_request_t *obj,
    uint32_t *flow_removed_mask_slave)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ASYNC_GET_REQUEST);
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
    of_wire_buffer_u32_get(wbuf, abs_offset, flow_removed_mask_slave);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set flow_removed_mask_slave in an object of type of_async_get_request.
 * @param obj Pointer to an object of type of_async_get_request.
 * @param flow_removed_mask_slave The value to write into the object
 */
void
of_async_get_request_flow_removed_mask_slave_set(
    of_async_get_request_t *obj,
    uint32_t flow_removed_mask_slave)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ASYNC_GET_REQUEST);
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
    of_wire_buffer_u32_set(wbuf, abs_offset, flow_removed_mask_slave);

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
of_async_set_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint8_t *)(buf + 0) = obj->version; /* version */
        *(uint8_t *)(buf + 1) = 0x1c; /* type */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_async_set of_async_set
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_async_set_push_wire_values(of_async_set_t *obj)
{

    of_async_set_push_wire_types(obj);

    /* Message obj; set length */
    of_message_t msg;

    if ((msg = OF_OBJECT_TO_MESSAGE(obj)) != NULL) {
        of_message_length_set(msg, obj->length);
    }

    return OF_ERROR_NONE;
}

/**
 * Create a new of_async_set object
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
 * \ingroup of_async_set
 */

of_async_set_t *
of_async_set_new(of_version_t version)
{
    of_async_set_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_ASYNC_SET] + of_object_extra_len[version][OF_ASYNC_SET];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_async_set_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_async_set_init(obj, version, bytes, 0);

    if (of_async_set_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_async_set.
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
of_async_set_init(of_async_set_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_ASYNC_SET] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_ASYNC_SET] + of_object_extra_len[version][OF_ASYNC_SET];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_ASYNC_SET;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_async_set_push_wire_types;

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
 * Create a new of_async_set object and bind it to an existing message
 *
 * @param msg The message to bind the new object to
 * @return Pointer to the newly create object or NULL on error
 *
 * \ingroup of_async_set
 */

of_async_set_t *
of_async_set_new_from_message(of_message_t msg)
{
    of_async_set_t *obj = NULL;
    of_version_t version;
    int length;

    if (msg == NULL) return NULL;

    version = of_message_version_get(msg);
    if (!OF_VERSION_OKAY(version)) return NULL;

    length = of_message_length_get(msg);

    if ((obj = (of_async_set_t *)of_object_new(-1)) == NULL) {
        return NULL;
    }

    of_async_set_init(obj, version, 0, 0);

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
 * Get xid from an object of type of_async_set.
 * @param obj Pointer to an object of type of_async_set.
 * @param xid Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_async_set_xid_get(
    of_async_set_t *obj,
    uint32_t *xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ASYNC_SET);
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
 * Set xid in an object of type of_async_set.
 * @param obj Pointer to an object of type of_async_set.
 * @param xid The value to write into the object
 */
void
of_async_set_xid_set(
    of_async_set_t *obj,
    uint32_t xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ASYNC_SET);
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
 * Get packet_in_mask_equal_master from an object of type of_async_set.
 * @param obj Pointer to an object of type of_async_set.
 * @param packet_in_mask_equal_master Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_async_set_packet_in_mask_equal_master_get(
    of_async_set_t *obj,
    uint32_t *packet_in_mask_equal_master)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ASYNC_SET);
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
    of_wire_buffer_u32_get(wbuf, abs_offset, packet_in_mask_equal_master);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set packet_in_mask_equal_master in an object of type of_async_set.
 * @param obj Pointer to an object of type of_async_set.
 * @param packet_in_mask_equal_master The value to write into the object
 */
void
of_async_set_packet_in_mask_equal_master_set(
    of_async_set_t *obj,
    uint32_t packet_in_mask_equal_master)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ASYNC_SET);
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
    of_wire_buffer_u32_set(wbuf, abs_offset, packet_in_mask_equal_master);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get packet_in_mask_slave from an object of type of_async_set.
 * @param obj Pointer to an object of type of_async_set.
 * @param packet_in_mask_slave Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_async_set_packet_in_mask_slave_get(
    of_async_set_t *obj,
    uint32_t *packet_in_mask_slave)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ASYNC_SET);
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
    of_wire_buffer_u32_get(wbuf, abs_offset, packet_in_mask_slave);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set packet_in_mask_slave in an object of type of_async_set.
 * @param obj Pointer to an object of type of_async_set.
 * @param packet_in_mask_slave The value to write into the object
 */
void
of_async_set_packet_in_mask_slave_set(
    of_async_set_t *obj,
    uint32_t packet_in_mask_slave)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ASYNC_SET);
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
    of_wire_buffer_u32_set(wbuf, abs_offset, packet_in_mask_slave);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get port_status_mask_equal_master from an object of type of_async_set.
 * @param obj Pointer to an object of type of_async_set.
 * @param port_status_mask_equal_master Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_async_set_port_status_mask_equal_master_get(
    of_async_set_t *obj,
    uint32_t *port_status_mask_equal_master)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ASYNC_SET);
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
    of_wire_buffer_u32_get(wbuf, abs_offset, port_status_mask_equal_master);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set port_status_mask_equal_master in an object of type of_async_set.
 * @param obj Pointer to an object of type of_async_set.
 * @param port_status_mask_equal_master The value to write into the object
 */
void
of_async_set_port_status_mask_equal_master_set(
    of_async_set_t *obj,
    uint32_t port_status_mask_equal_master)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ASYNC_SET);
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
    of_wire_buffer_u32_set(wbuf, abs_offset, port_status_mask_equal_master);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get port_status_mask_slave from an object of type of_async_set.
 * @param obj Pointer to an object of type of_async_set.
 * @param port_status_mask_slave Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_async_set_port_status_mask_slave_get(
    of_async_set_t *obj,
    uint32_t *port_status_mask_slave)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ASYNC_SET);
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
    of_wire_buffer_u32_get(wbuf, abs_offset, port_status_mask_slave);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set port_status_mask_slave in an object of type of_async_set.
 * @param obj Pointer to an object of type of_async_set.
 * @param port_status_mask_slave The value to write into the object
 */
void
of_async_set_port_status_mask_slave_set(
    of_async_set_t *obj,
    uint32_t port_status_mask_slave)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ASYNC_SET);
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
    of_wire_buffer_u32_set(wbuf, abs_offset, port_status_mask_slave);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get flow_removed_mask_equal_master from an object of type of_async_set.
 * @param obj Pointer to an object of type of_async_set.
 * @param flow_removed_mask_equal_master Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_async_set_flow_removed_mask_equal_master_get(
    of_async_set_t *obj,
    uint32_t *flow_removed_mask_equal_master)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ASYNC_SET);
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
    of_wire_buffer_u32_get(wbuf, abs_offset, flow_removed_mask_equal_master);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set flow_removed_mask_equal_master in an object of type of_async_set.
 * @param obj Pointer to an object of type of_async_set.
 * @param flow_removed_mask_equal_master The value to write into the object
 */
void
of_async_set_flow_removed_mask_equal_master_set(
    of_async_set_t *obj,
    uint32_t flow_removed_mask_equal_master)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ASYNC_SET);
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
    of_wire_buffer_u32_set(wbuf, abs_offset, flow_removed_mask_equal_master);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get flow_removed_mask_slave from an object of type of_async_set.
 * @param obj Pointer to an object of type of_async_set.
 * @param flow_removed_mask_slave Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_async_set_flow_removed_mask_slave_get(
    of_async_set_t *obj,
    uint32_t *flow_removed_mask_slave)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ASYNC_SET);
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
    of_wire_buffer_u32_get(wbuf, abs_offset, flow_removed_mask_slave);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set flow_removed_mask_slave in an object of type of_async_set.
 * @param obj Pointer to an object of type of_async_set.
 * @param flow_removed_mask_slave The value to write into the object
 */
void
of_async_set_flow_removed_mask_slave_set(
    of_async_set_t *obj,
    uint32_t flow_removed_mask_slave)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ASYNC_SET);
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
    of_wire_buffer_u32_set(wbuf, abs_offset, flow_removed_mask_slave);

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
of_bsn_arp_idle_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint8_t *)(buf + 0) = obj->version; /* version */
        *(uint8_t *)(buf + 1) = 0x4; /* type */
        *(uint32_t *)(buf + 8) = U32_HTON(0x5c16c7); /* experimenter */
        *(uint32_t *)(buf + 12) = U32_HTON(0x3c); /* subtype */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_bsn_arp_idle of_bsn_arp_idle
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_bsn_arp_idle_push_wire_values(of_bsn_arp_idle_t *obj)
{

    of_bsn_arp_idle_push_wire_types(obj);

    /* Message obj; set length */
    of_message_t msg;

    if ((msg = OF_OBJECT_TO_MESSAGE(obj)) != NULL) {
        of_message_length_set(msg, obj->length);
    }

    return OF_ERROR_NONE;
}

/**
 * Create a new of_bsn_arp_idle object
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
 * \ingroup of_bsn_arp_idle
 */

of_bsn_arp_idle_t *
of_bsn_arp_idle_new(of_version_t version)
{
    of_bsn_arp_idle_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_BSN_ARP_IDLE] + of_object_extra_len[version][OF_BSN_ARP_IDLE];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_bsn_arp_idle_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_bsn_arp_idle_init(obj, version, bytes, 0);

    if (of_bsn_arp_idle_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_bsn_arp_idle.
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
of_bsn_arp_idle_init(of_bsn_arp_idle_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_BSN_ARP_IDLE] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_BSN_ARP_IDLE] + of_object_extra_len[version][OF_BSN_ARP_IDLE];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_BSN_ARP_IDLE;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_bsn_arp_idle_push_wire_types;

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
 * Create a new of_bsn_arp_idle object and bind it to an existing message
 *
 * @param msg The message to bind the new object to
 * @return Pointer to the newly create object or NULL on error
 *
 * \ingroup of_bsn_arp_idle
 */

of_bsn_arp_idle_t *
of_bsn_arp_idle_new_from_message(of_message_t msg)
{
    of_bsn_arp_idle_t *obj = NULL;
    of_version_t version;
    int length;

    if (msg == NULL) return NULL;

    version = of_message_version_get(msg);
    if (!OF_VERSION_OKAY(version)) return NULL;

    length = of_message_length_get(msg);

    if ((obj = (of_bsn_arp_idle_t *)of_object_new(-1)) == NULL) {
        return NULL;
    }

    of_bsn_arp_idle_init(obj, version, 0, 0);

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
 * Get xid from an object of type of_bsn_arp_idle.
 * @param obj Pointer to an object of type of_bsn_arp_idle.
 * @param xid Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_bsn_arp_idle_xid_get(
    of_bsn_arp_idle_t *obj,
    uint32_t *xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_BSN_ARP_IDLE);
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
 * Set xid in an object of type of_bsn_arp_idle.
 * @param obj Pointer to an object of type of_bsn_arp_idle.
 * @param xid The value to write into the object
 */
void
of_bsn_arp_idle_xid_set(
    of_bsn_arp_idle_t *obj,
    uint32_t xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_BSN_ARP_IDLE);
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
 * Get experimenter from an object of type of_bsn_arp_idle.
 * @param obj Pointer to an object of type of_bsn_arp_idle.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_bsn_arp_idle_experimenter_get(
    of_bsn_arp_idle_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_BSN_ARP_IDLE);
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
 * Set experimenter in an object of type of_bsn_arp_idle.
 * @param obj Pointer to an object of type of_bsn_arp_idle.
 * @param experimenter The value to write into the object
 */
void
of_bsn_arp_idle_experimenter_set(
    of_bsn_arp_idle_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_BSN_ARP_IDLE);
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
 * Get subtype from an object of type of_bsn_arp_idle.
 * @param obj Pointer to an object of type of_bsn_arp_idle.
 * @param subtype Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_bsn_arp_idle_subtype_get(
    of_bsn_arp_idle_t *obj,
    uint32_t *subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_BSN_ARP_IDLE);
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
 * Set subtype in an object of type of_bsn_arp_idle.
 * @param obj Pointer to an object of type of_bsn_arp_idle.
 * @param subtype The value to write into the object
 */
void
of_bsn_arp_idle_subtype_set(
    of_bsn_arp_idle_t *obj,
    uint32_t subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_BSN_ARP_IDLE);
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
 * Get vlan_vid from an object of type of_bsn_arp_idle.
 * @param obj Pointer to an object of type of_bsn_arp_idle.
 * @param vlan_vid Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_bsn_arp_idle_vlan_vid_get(
    of_bsn_arp_idle_t *obj,
    uint16_t *vlan_vid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_BSN_ARP_IDLE);
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
    of_wire_buffer_u16_get(wbuf, abs_offset, vlan_vid);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set vlan_vid in an object of type of_bsn_arp_idle.
 * @param obj Pointer to an object of type of_bsn_arp_idle.
 * @param vlan_vid The value to write into the object
 */
void
of_bsn_arp_idle_vlan_vid_set(
    of_bsn_arp_idle_t *obj,
    uint16_t vlan_vid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_BSN_ARP_IDLE);
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
    of_wire_buffer_u16_set(wbuf, abs_offset, vlan_vid);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get ipv4_addr from an object of type of_bsn_arp_idle.
 * @param obj Pointer to an object of type of_bsn_arp_idle.
 * @param ipv4_addr Pointer to the child object of type
 * of_ipv4_t to be filled out.
 *
 */
void
of_bsn_arp_idle_ipv4_addr_get(
    of_bsn_arp_idle_t *obj,
    of_ipv4_t *ipv4_addr)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_BSN_ARP_IDLE);
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
    of_wire_buffer_ipv4_get(wbuf, abs_offset, ipv4_addr);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set ipv4_addr in an object of type of_bsn_arp_idle.
 * @param obj Pointer to an object of type of_bsn_arp_idle.
 * @param ipv4_addr The value to write into the object
 */
void
of_bsn_arp_idle_ipv4_addr_set(
    of_bsn_arp_idle_t *obj,
    of_ipv4_t ipv4_addr)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_BSN_ARP_IDLE);
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
    of_wire_buffer_ipv4_set(wbuf, abs_offset, ipv4_addr);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}
