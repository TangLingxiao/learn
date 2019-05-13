
import xml.etree.ElementTree as ET
 
def _serialize_xml(write, elem, encoding, qnames, namespaces):
    tag = elem.tag
    text = elem.text
    if tag is ET.Comment:
        write("<!--%s-->" % ET._encode(text, encoding))
    elif tag is ET.ProcessingInstruction:
        write("<?%s?>" % ET._encode(text, encoding))
    else:
        tag = qnames[tag]
        if tag is None:
            if text:
                write(ET._escape_cdata(text, encoding))
            for e in elem:
                _serialize_xml(write, e, encoding, qnames, None)
        else:
            write("<" + tag)
            items = elem.items()
            if items or namespaces:
                if namespaces:
                    for v, k in sorted(namespaces.items(),
                            key=lambda x: x[1]):  # sort on prefix
                        if k:
                            k = ":" + k
                        write(" xmlns%s=\"%s\"" % (
                            k.encode(encoding),
                            ET._escape_attrib(v, encoding)
                        ))
                # for k, v in sorted(items):  # lexical order
                for k, v in items:  # Monkey patch
                    if isinstance(k, ET.QName):
                        k = k.text
                    if isinstance(v, ET.QName):
                        v = qnames[v.text]
                    else:
                        v = ET._escape_attrib(v, encoding)
                    write(" %s=\"%s\"" % (qnames[k], v))
            if text or len(elem):
                write(">")
                if text:
                    write(ET._escape_cdata(text, encoding))
                for e in elem:
                    _serialize_xml(write, e, encoding, qnames, None)
                write("</" + tag + ">")
            else:
                write("/>")
    if elem.tail:
        write(ET._escape_cdata(elem.tail, encoding))
ET._serialize_xml = _serialize_xml
 
 
def _serialize_text(write, elem, encoding):
    for part in elem.itertext():
        write(part.encode(encoding))
    if elem.tail:
        write(elem.tail.encode(encoding))
ET._serialize_text = _serialize_text
 
_serialize = {
    "xml": _serialize_xml,
    "html": ET._serialize_html,
    "text": _serialize_text,
# this optional method is imported at the end of the module
#   "c14n": _serialize_c14n,
}
ET._serialize = _serialize
 
def write(self, file_or_filename,
          # keyword arguments
          encoding=None,
          xml_declaration=None,
          default_namespace=None,
          method=None):
    # assert self._root is not None
    if not method:
        method = "xml"
    elif method not in _serialize:
        # FIXME: raise an ImportError for c14n if ElementC14N is missing?
        raise ValueError("unknown method %r" % method)
    if hasattr(file_or_filename, "write"):
        file = file_or_filename
    else:
        file = open(file_or_filename, "wb")
        # file = codecs.open(file_or_filename,"w","utf-8-sig")
    write = file.write
    if not encoding:
        if method == "c14n":
            encoding = "utf-8"
        else:
            encoding = "us-ascii"
    elif xml_declaration or (xml_declaration is None and
                                     encoding not in ("utf-8", "us-ascii")):
        if method == "xml":
            write('<?xml version="1.0" encoding="%s" standalone="yes"?>\r\n' % encoding)
    if method == "text":
        _serialize_text(write, self._root, encoding)
    else:
        qnames, namespaces = ET._namespaces(
            self._root, encoding, default_namespace
        )
        serialize = _serialize[method]
        serialize(write, self._root, encoding, qnames, namespaces)
    if file_or_filename is not file:
        file.close()
ET.ElementTree.write = write
 
 
 
from collections import OrderedDict
class OrderedXMLTreeBuilder(ET.XMLTreeBuilder):
    def _start_list(self, tag, attrib_in):
        fixname = self._fixname
        tag = fixname(tag)
        attrib = OrderedDict()
        if attrib_in:
            for i in range(0, len(attrib_in), 2):
                attrib[fixname(attrib_in[i])] = self._fixtext(attrib_in[i + 1])
        return self._target.start(tag, attrib)