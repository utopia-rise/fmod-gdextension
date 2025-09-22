#!/usr/bin/env python3
"""
Test script to verify FMOD GDExtension documentation is properly structured.
"""

import os
import xml.etree.ElementTree as ET

def test_documentation():
    """Test all documentation files for proper structure."""
    doc_dir = "demo/addons/fmod/doc_classes"
    
    if not os.path.exists(doc_dir):
        print("❌ Documentation directory not found!")
        return False
    
    xml_files = [f for f in os.listdir(doc_dir) if f.endswith('.xml')]
    
    if not xml_files:
        print("❌ No XML documentation files found!")
        return False
    
    print(f"Found {len(xml_files)} documentation files:")
    
    valid_files = 0
    total_methods = 0
    
    for filename in sorted(xml_files):
        filepath = os.path.join(doc_dir, filename)
        try:
            tree = ET.parse(filepath)
            root = tree.getroot()
            
            # Check basic structure
            if root.tag != 'class':
                print(f"❌ {filename}: Root element should be 'class'")
                continue
                
            class_name = root.get('name')
            inherits = root.get('inherits')
            
            # Count methods
            methods = root.findall('.//method')
            method_count = len(methods)
            total_methods += method_count
            
            # Count members
            members = root.findall('.//member')
            member_count = len(members)
            
            # Count signals
            signals = root.findall('.//signal')
            signal_count = len(signals)
            
            print(f"✓ {class_name} ({method_count} methods, {member_count} properties, {signal_count} signals)")
            valid_files += 1
            
        except ET.ParseError as e:
            print(f"❌ {filename}: XML parse error - {e}")
        except Exception as e:
            print(f"❌ {filename}: Error - {e}")
    
    print(f"\n📊 Summary:")
    print(f"   Valid files: {valid_files}/{len(xml_files)}")
    print(f"   Total methods documented: {total_methods}")
    
    # Check gdextension configuration
    gdext_path = "demo/addons/fmod/fmod.gdextension"
    if os.path.exists(gdext_path):
        with open(gdext_path, 'r') as f:
            content = f.read()
            if '[documentation]' in content and 'on_classes' in content:
                print("✓ GDExtension configuration includes documentation section")
            else:
                print("❌ GDExtension configuration missing documentation section")
    else:
        print("❌ GDExtension configuration file not found")
    
    return valid_files == len(xml_files)

if __name__ == "__main__":
    print("🔍 Testing FMOD GDExtension Documentation\n")
    success = test_documentation()
    
    if success:
        print("\n✅ All documentation tests passed!")
        print("📚 Documentation is ready for use in Godot 4.3+")
    else:
        print("\n❌ Some documentation tests failed!")
        exit(1)