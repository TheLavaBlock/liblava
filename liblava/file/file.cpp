/**
 * @file         liblava/file/file.cpp
 * @brief        File access
 * @authors      Lava Block OÜ and contributors
 * @copyright    Copyright (c) 2018-present, MIT License
 */

#include "liblava/file/file.hpp"
#include "physfs.h"

namespace lava {

//-----------------------------------------------------------------------------
file::file(string_ref p, file_mode m) {
    open(p, m);
}

//-----------------------------------------------------------------------------
file::~file() {
    close();
}

//-----------------------------------------------------------------------------
bool file::open(string_ref p, file_mode m) {
    if (p.empty())
        return false;

    path = p;
    mode = m;

    if (mode == file_mode::write)
        fs_file = PHYSFS_openWrite(str(path));
    else
        fs_file = PHYSFS_openRead(str(path));

    if (fs_file) {
        type = file_type::fs;
    } else {
        if (mode == file_mode::write) {
            o_stream = std::ofstream(path,
                                     std::ios::binary);
            if (o_stream.is_open())
                type = file_type::f_stream;
        } else {
            i_stream = std::ifstream(path,
                                     std::ios::binary);
            if (i_stream.is_open())
                type = file_type::f_stream;

            i_stream.seekg(0, i_stream.beg);
        }
    }

    return opened();
}

//-----------------------------------------------------------------------------
void file::close() {
    if (type == file_type::fs) {
        PHYSFS_close(fs_file);
    } else if (type == file_type::f_stream) {
        if (mode == file_mode::write)
            o_stream.close();
        else
            i_stream.close();
    }
}

//-----------------------------------------------------------------------------
bool file::opened() const {
    if (type == file_type::fs) {
        return fs_file != nullptr;
    } else if (type == file_type::f_stream) {
        if (mode == file_mode::write)
            return o_stream.is_open();
        else
            return i_stream.is_open();
    }

    return false;
}

//-----------------------------------------------------------------------------
i64 file::get_size() const {
    if (type == file_type::fs) {
        return PHYSFS_fileLength(fs_file);
    } else if (type == file_type::f_stream) {
        if (mode == file_mode::write) {
            auto current = o_stream.tellp();
            o_stream.seekp(0, o_stream.end);
            auto result = o_stream.tellp();
            o_stream.seekp(current);
            return result;
        } else {
            auto current = i_stream.tellg();
            i_stream.seekg(0, i_stream.end);
            auto result = i_stream.tellg();
            i_stream.seekg(current);
            return result;
        }
    }

    return file_error_result;
}

//-----------------------------------------------------------------------------
i64 file::read(data::ptr data, ui64 size) {
    if (mode == file_mode::write)
        return file_error_result;

    if (type == file_type::fs)
        return PHYSFS_readBytes(fs_file, data, size);
    else if (type == file_type::f_stream)
        return i_stream.read(data, size).gcount();

    return file_error_result;
}

//-----------------------------------------------------------------------------
i64 file::write(data::c_ptr data, ui64 size) {
    if (mode != file_mode::write)
        return file_error_result;

    if (type == file_type::fs) {
        return PHYSFS_writeBytes(fs_file, data, size);
    } else if (type == file_type::f_stream) {
        o_stream.write(data, size);
        return to_i64(size);
    }

    return file_error_result;
}

//-----------------------------------------------------------------------------
i64 file::seek(ui64 position) {
    if (type == file_type::fs) {
        return PHYSFS_seek(fs_file, position);
    } else if (type == file_type::f_stream) {
        if (mode == file_mode::write)
            o_stream.seekp(position);
        else
            i_stream.seekg(position);

        return tell();
    }

    return file_error_result;
}

//-----------------------------------------------------------------------------
i64 file::tell() const {
    if (type == file_type::fs) {
        return PHYSFS_tell(fs_file);
    } else if (type == file_type::f_stream) {
        if (mode == file_mode::write)
            return to_i64(o_stream.tellp());
        else
            return to_i64(i_stream.tellg());
    }

    return file_error_result;
}

} // namespace lava
