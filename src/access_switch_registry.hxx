#pragma once

#include <string>
#include <unordered_map>

namespace AccessUtils::Restore
{
    enum class Neo4jNode
    {
        body,
        lump,
        shell,
        subshell,
        wire,
        face,
        loop,
        coedge,
        edge,
        vertex,
        pcurve,
        apoint,
        straight_curve,
        ellipse_curve,
        helix_curve,
        intcurve_curve,
        plane_surface,
        sphere_surface,
        torus_surface,
        cone_surface,
        spline_surface,
        transform,
        spl_sur,
        int_cur,
        par_cur
    };

    extern const std::unordered_map<std::string, Neo4jNode> Neo4jNode_str2enum;

    enum class Neo4jEdge
    {
        body_lump_ptr,
        body_wire_ptr,
        body_transform_ptr,
        lump_next_ptr,
        lump_shell_ptr,
        lump_body_ptr,
        shell_next_ptr,
        shell_subshell_ptr,
        shell_face_ptr,
        shell_wire_ptr,
        shell_lump_ptr,
        subshell_parent_ptr,
        subshell_sibling_ptr,
        subshell_child_ptr,
        subshell_face_ptr,
        subshell_wire_ptr,
        wire_next_ptr,
        wire_coedge_ptr,
        wire_owner_ptr,
        wire_subshell_ptr,
        face_next_ptr,
        face_loop_ptr,
        face_shell_ptr,
        face_subshell_ptr,
        face_geometry_ptr,
        loop_next_ptr,
        loop_start_ptr,
        loop_face_ptr,
        coedge_next_ptr,
        coedge_previous_ptr,
        coedge_partner_ptr,
        coedge_edge_ptr,
        coedge_owner_ptr,
        coedge_geometry_ptr,
        edge_start_ptr,
        edge_end_ptr,
        edge_coedge_ptr,
        edge_geometry_ptr,
        vertex_edge_ptr,
        vertex_geometry_ptr,
        pcurve_ref_curve_ptr,
        pcurve_fit_ptr,
        spline_surface_spl_ptr,
        intcurve_curve_fit_ptr,
        int_cur_surf1_spl_ptr,
        int_cur_surf2_spl_ptr,
        par_cur_surf_spl_ptr
    };

    extern const std::unordered_map<std::string, Neo4jEdge> Neo4jEdge_str2enum;
}
