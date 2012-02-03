/******************************************************************************
 ** Copyright (c) 2010 Seed Developers
 ** All rights reserved
 ** Contact: licensing@seedframework.org
 ** Website: http://www.seedframework.org
 
 ** This file is part of the Seed Framework.
 
 ** Commercial Usage
 ** Seed Framework is available under proprietary license for those who cannot,
 ** or choose not to, use LGPL and GPL code in their projects (eg. iPhone,
 ** Nintendo Wii and others).
 
 ** GNU Lesser General Public License Usage
 ** Alternatively, this file may be used under the terms of the GNU Lesser
 ** General Public License version 2.1 as published by the Free Software
 ** Foundation and appearing in the file LICENSE.LGPL included in the
 ** packaging of this file.  Please review the following information to
 ** ensure the GNU Lesser General Public License version 2.1 requirements
 ** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
 **
 ** In addition, as a special exception, Seed developers gives you certain
 ** additional rights. These rights are described in the Seed Framework LGPL
 ** Exception version 1.1, included in the file LGPL_EXCEPTION.txt in this
 ** package.
 **
 ** If you have questions regarding the use of this file, please contact
 ** Seed developers at licensing@seedframework.org.
 **
 *****************************************************************************/

#include "Key.h"


namespace Seed {


Key::Key(int value)
	: iValue(value)
{
}

Key::~Key()
{
}

bool Key::IsCharacter() const
{
	return (iValue >= 32 && iValue <= 126) || (iValue >= 162 && iValue <= 255) || (iValue == 9);
}

bool Key::IsNumber() const
{
	return iValue >= 48 && iValue <= 57;
}

bool Key::IsLetter() const
{
	return (((iValue >= 65 && iValue <= 90) || (iValue >= 97 && iValue <= 122) || (iValue >= 192 && iValue <= 255)) && (iValue != 215) && (iValue != 247));
}

u32 Key::GetValue() const
{
	return iValue;
}

bool Key::operator==(const Key& key) const
{
	return iValue == key.iValue;
}

bool Key::operator!=(const Key& key) const
{
	return (iValue != key.iValue);
}


} // namespace
