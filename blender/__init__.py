import bpy
import os
import shutil
from bpy.props import (
	StringProperty,
	BoolProperty
)

from importlib import reload
from hellgun import BinaryStream
from hellgun import MeshExporter
from hellgun import LevelExporter

bl_info = {
	"name": "Hellgun Blender Tools",
	"blender": (2, 80, 0),
	"category": "Object"
}

class HellgunMeshExport(bpy.types.Operator):
	bl_idname = "hellgun.meshexport"
	bl_description = "Export to Hellgun mesh file"
	bl_label = "Hellgun Mesh (.msh)"
	bl_options = { "REGISTER" }

	filepath: StringProperty(subtype="FILE_PATH")
	only_selected: BoolProperty(name="Only Selected", default=False)

	def execute(self, context):
		self.filepath =  bpy.path.ensure_ext(self.filepath, ".msh");
		obj = context.active_object;

		MeshExporter.export_mesh(obj, self.filepath);
		return { "FINISHED" };

	def invoke(self, context, event):
		obj = context.active_object;
		self.filepath = obj.name + ".msh";

		wnd_manager = context.window_manager;
		wnd_manager.fileselect_add(self);
		return { "RUNNING_MODAL" };

class HellgunLevelExport(bpy.types.Operator):
	bl_idname = "hellgun.levelexport"
	bl_description = "Export Hellgun level"
	bl_label = "Hellgun Level (.lvl)"
	bl_options = { "REGISTER" }

	filepath: StringProperty(subtype="FILE_PATH", default="level.lvl")
	level_collection_name: StringProperty(name="Level Collection Name", default="level")

	def execute(self, context):
		self.filepath =  bpy.path.ensure_ext(self.filepath, ".lvl");

		# Find the specified level collection
		root_collection = context.scene.collection;
		level_collection = None;

		for child in root_collection.children:
			if child.name == self.level_collection_name:
				level_collection = child;
				break;

		# Warning if the level collection was not found
		if level_collection == None:
			self.report({"WARNING"}, "The specified level collection wasn't found");
			return { "CANCELLED" };

		# Export level
		LevelExporter.export_level(level_collection, self.filepath);

		# Make the subfolder for all the meshes, or delete it if it already exists
		(dir, file) = os.path.split(self.filepath);
		(subdir, _) = os.path.splitext(file);
		subdir = os.path.join(dir, subdir);

		if os.path.isdir(subdir):
			shutil.rmtree(subdir);

		os.mkdir(subdir);

		for object in level_collection.objects:
			if object.type != "MESH":
				continue;

			mesh_path = os.path.join(subdir, object.name + ".msh");
			MeshExporter.export_mesh(object, mesh_path);

		return { "FINISHED" }

	def invoke(self, context, event):
		self.filepath = "level.lvl";

		wnd_manager = context.window_manager;
		wnd_manager.fileselect_add(self);

		return { "RUNNING_MODAL" };

class HellgunEditorPanel(bpy.types.Panel):
	bl_label = "Hellgun"
	bl_idname = "OBJECT_PT_Hellgun"
	bl_space_type = "PROPERTIES"
	bl_region_type = "WINDOW"
	bl_context = "object"

	def draw(self, context):
		layout = self.layout;
		obj = context.object;

		row = layout.row();
		row.prop(obj, "hellgun_render");
		row.prop(obj, "hellgun_collision");

		row = layout.row();
		row.prop(obj, "hellgun_material");

def mesh_export_func(self, context):
	self.layout.operator(HellgunMeshExport.bl_idname);
def level_export_func(self, context):
	self.layout.operator(HellgunLevelExport.bl_idname);

def on_render_changed(self, context):
	print("'%s' changed render to '%d'" % (self.name, self.hellgun_render));

	if self.hellgun_render:
		self.display_type = "SOLID";
	else:
		self.display_type = "WIRE";

def on_collision_changed(self, context):
	print("'%s' changed collision to '%d'" % (self.name, self.hellgun_collision));
	self.show_bounds = self.hellgun_collision;

def register():
	reload(BinaryStream);
	reload(MeshExporter);
	reload(LevelExporter);

	bpy.types.Object.hellgun_render = bpy.props.BoolProperty(name="Render", default=True, update=on_render_changed);
	bpy.types.Object.hellgun_collision = bpy.props.BoolProperty(name="Collision", default=False, update=on_collision_changed);
	bpy.types.Object.hellgun_material = bpy.props.StringProperty(name="Material", default="material/prop.mat");

	bpy.utils.register_class(HellgunMeshExport);
	bpy.utils.register_class(HellgunLevelExport);
	bpy.utils.register_class(HellgunEditorPanel);
	bpy.types.TOPBAR_MT_file_export.append(mesh_export_func);
	bpy.types.TOPBAR_MT_file_export.append(level_export_func);

def unregister():
	bpy.utils.unregister_class(HellgunMeshExport);
	bpy.utils.unregister_class(HellgunLevelExport);
	bpy.utils.unregister_class(HellgunEditorPanel);
	bpy.types.TOPBAR_MT_file_export.remove(mesh_export_func);
	bpy.types.TOPBAR_MT_file_export.remove(level_export_func);
