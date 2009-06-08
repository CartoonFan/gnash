// NativeWindowInitOptions_as.hx:  ActionScript 3 "NativeWindowInitOptions" class, for Gnash.
//
// Generated by gen-as3.sh on: 20090514 by "rob". Remove this
// after any hand editing loosing changes.
//
//   Copyright (C) 2009 Free Software Foundation, Inc.
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//

// This test case must be processed by CPP before compiling to include the
//  DejaGnu.hx header file for the testing framework support.

#if flash9
import flash.display.NativeWindowInitOptions;
import flash.display.MovieClip;
#else
import flash.NativeWindowInitOptions;
import flash.MovieClip;
#end
import flash.Lib;
import Type;

// import our testing API
import DejaGnu;

// Class must be named with the _as suffix, as that's the same name as the file.
class NativeWindowInitOptions_as {
    static function main() {
        var x1:NativeWindowInitOptions = new NativeWindowInitOptions();

        // Make sure we actually get a valid class        
        if (x1 != null) {
            DejaGnu.pass("NativeWindowInitOptions class exists");
        } else {
            DejaGnu.fail("NativeWindowInitOptions lass doesn't exist");
        }
// Tests to see if all the properties exist. All these do is test for
// existance of a property, and don't test the functionality at all. This
// is primarily useful only to test completeness of the API implementation.
	if (x1.maximizable == false) {
	    DejaGnu.pass("NativeWindowInitOptions::maximizable property exists");
	} else {
	    DejaGnu.fail("NativeWindowInitOptions::maximizable property doesn't exist");
	}
	if (x1.minimizable == false) {
	    DejaGnu.pass("NativeWindowInitOptions::minimizable property exists");
	} else {
	    DejaGnu.fail("NativeWindowInitOptions::minimizable property doesn't exist");
	}
	if (x1.resizable == false) {
	    DejaGnu.pass("NativeWindowInitOptions::resizable property exists");
	} else {
	    DejaGnu.fail("NativeWindowInitOptions::resizable property doesn't exist");
	}
	if (x1.systemChrome == null) {
	    DejaGnu.pass("NativeWindowInitOptions::systemChrome property exists");
	} else {
	    DejaGnu.fail("NativeWindowInitOptions::systemChrome property doesn't exist");
	}
	if (x1.transparent == false) {
	    DejaGnu.pass("NativeWindowInitOptions::transparent property exists");
	} else {
	    DejaGnu.fail("NativeWindowInitOptions::transparent property doesn't exist");
	}
	if (x1.type == null) {
	    DejaGnu.pass("NativeWindowInitOptions::type property exists");
	} else {
	    DejaGnu.fail("NativeWindowInitOptions::type property doesn't exist");
	}

// Tests to see if all the methods exist. All these do is test for
// existance of a method, and don't test the functionality at all. This
// is primarily useful only to test completeness of the API implementation.
	if (x1.NativeWindowInitOptions == NativeWindowInitOptions) {
	    DejaGnu.pass("NativeWindowInitOptions::NativeWindowInitOptions() method exists");
	} else {
	    DejaGnu.fail("NativeWindowInitOptions::NativeWindowInitOptions() method doesn't exist");
	}

        // Call this after finishing all tests. It prints out the totals.
        DejaGnu.done();
    }
}

// local Variables:
// mode: C++
// indent-tabs-mode: t
// End:
