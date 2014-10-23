/*
 * #%L
 * OME-BIOFORMATS C++ library for image IO.
 * %%
 * Copyright © 2006 - 2013 Open Microscopy Environment:
 *   - Massachusetts Institute of Technology
 *   - National Institutes of Health
 *   - University of Dundee
 *   - Board of Regents of the University of Wisconsin-Madison
 *   - Glencoe Software, Inc.
 * %%
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * The views and conclusions contained in the software and documentation are
 * those of the authors and should not be interpreted as representing official
 * policies, either expressed or implied, of any organization.
 * #L%
 */

#include <stdexcept>
#include <vector>

#include <ome/bioformats/VariantPixelBuffer.h>
#include <ome/bioformats/in/MinimalTIFFReader.h>

#include <ome/test/config.h>

#include <gtest/gtest.h>

using ome::bioformats::dimension_size_type;
using ome::bioformats::VariantPixelBuffer;
using ome::bioformats::in::MinimalTIFFReader;

class TIFFTestParameters
{
public:

  std::string file;
  dimension_size_type series;

  TIFFTestParameters(const std::string& file,
                     dimension_size_type series):
    file(file),
    series(series)
  {}
};

template<class charT, class traits>
inline std::basic_ostream<charT,traits>&
operator<< (std::basic_ostream<charT,traits>& os,
            const TIFFTestParameters& tp)
{
  os << tp.file;

  return os;
}

class TIFFTest : public ::testing::TestWithParam<TIFFTestParameters>
{
public:
  MinimalTIFFReader tiff;
};

TEST_P(TIFFTest, setId)
{
  const TIFFTestParameters& params = GetParam();

  tiff.setId(params.file);
}

TEST_P(TIFFTest, seriesCount)
{
  const TIFFTestParameters& params = GetParam();

  tiff.setId(params.file);
  EXPECT_EQ(params.series, tiff.getSeriesCount());
}

TEST_P(TIFFTest, setSeries)
{
  const TIFFTestParameters& params = GetParam();

  tiff.setId(params.file);

  EXPECT_EQ(0, tiff.getSeries());

  for (dimension_size_type s = 0; s < tiff.getSeriesCount(); ++s)
    {
      EXPECT_NO_THROW(tiff.setSeries(s));
      EXPECT_EQ(s, tiff.getSeries());
    }

  EXPECT_THROW(tiff.setSeries(tiff.getSeriesCount() + 1), std::logic_error);
}

TEST_P(TIFFTest, openBytes)
{
  const TIFFTestParameters& params = GetParam();

  tiff.setId(params.file);
  for (dimension_size_type s = 0; s < tiff.getSeriesCount(); ++s)
    {
      VariantPixelBuffer buf;
      EXPECT_NO_THROW(tiff.setSeries(s));
      EXPECT_EQ(s, tiff.getSeries());
      EXPECT_NO_THROW(tiff.openBytes(0, buf));
    }
}

namespace
{

  std::vector<TIFFTestParameters>
  init_params()
  {
    std::vector<TIFFTestParameters> params;

    std::string filename(PROJECT_SOURCE_DIR "/components/specification/samples/2010-06/18x24y5z1t2c8b-text.ome.tiff");

    TIFFTestParameters p(filename, 10);

    params.push_back(p);

    return params;
  }

}

std::vector<TIFFTestParameters> params(init_params());

// Disable missing-prototypes warning for INSTANTIATE_TEST_CASE_P;
// this is solely to work around a missing prototype in gtest.
#ifdef __GNUC__
#  if defined __clang__ || defined __APPLE__
#    pragma GCC diagnostic ignored "-Wmissing-prototypes"
#  endif
#  pragma GCC diagnostic ignored "-Wmissing-declarations"
#endif

INSTANTIATE_TEST_CASE_P(TIFFVariants, TIFFTest, ::testing::ValuesIn(params));
