function createObject(_type, _parent)
{
    var component = Qt.createComponent(_type + ".qml");
    var object = component.createObject(_parent);

    return object;
}

function deleteObject(_object)
{
    _object.destroy();
}
