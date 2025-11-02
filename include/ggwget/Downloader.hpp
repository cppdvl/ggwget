#pragma once

#include <filesystem>
#include <iosfwd>
#include <string>

namespace ggwget {

/**
 * Download the resource at `url` and write the raw response bytes to `output`.
 *
 * Throws std::runtime_error on failure.
 */
void download_to_stream(const std::string& url, std::ostream& output);

/**
 * Download the resource at `url` and store it in `path`.
 *
 * Throws std::runtime_error on failure.
 */
void download_to_file(const std::string& url, const std::filesystem::path& path);

}  // namespace ggwget
