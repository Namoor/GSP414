using UnityEngine;
using System.Collections;

public class WuerfelGen : MonoBehaviour {
	
	public MeshFilter m_MeshFilter;
	
	// Use this for initialization
	void Start () {
		
		Mesh _CubeMesh = new Mesh ();
		
		// Mesh mit daten befüllen
		
		Vector3[] _VerticePositions = new Vector3[24];
		Vector3[] _Normals = new Vector3[24]; // Eine Normal für jede Vertice
		Vector2[] _TexCoords = new Vector2[24]; // Eine TexCoord für jede Vertice

		int[] _TriangleIndices = new int[12 * 3]; // 3 Indices pro Triangle

		/*
		_VerticePositions [0] = 0.5f * new Vector3 (-1, -1, -1);
		_VerticePositions [1] = 0.5f * new Vector3 (1, -1, -1);
		_VerticePositions [2] = 0.5f * new Vector3 (-1, -1, 1);
		_VerticePositions [3] = 0.5f * new Vector3 (1, -1, 1);
		_VerticePositions [4] = 0.5f * new Vector3 (-1, 1, -1);
		_VerticePositions [5] = 0.5f * new Vector3 (1, 1, -1);
		_VerticePositions [6] = 0.5f * new Vector3 (-1, 1, 1);
		_VerticePositions [7] = 0.5f * new Vector3 (1, 1, 1);*/

		// Vorne
		_VerticePositions [0] = 0.5f * new Vector3 (-1, -1, -1);
		_VerticePositions [1] = 0.5f * new Vector3 (1, -1, -1);
		_VerticePositions [2] = 0.5f * new Vector3 (-1, 1, -1);
		_VerticePositions [3] = 0.5f * new Vector3 (1, 1, -1);

		// Rechts
		_VerticePositions [4] = 0.5f * new Vector3 (1, -1, -1);
		_VerticePositions [5] = 0.5f * new Vector3 (1, -1, 1);
		_VerticePositions [6] = 0.5f * new Vector3 (1, 1, -1);
		_VerticePositions [7] = 0.5f * new Vector3 (1, 1, 1);

		//Oben
		_VerticePositions [8] = 0.5f * new Vector3 (-1, 1, -1);
		_VerticePositions [9] = 0.5f * new Vector3 (1, 1, -1);
		_VerticePositions [10] = 0.5f * new Vector3 (-1, 1, 1);
		_VerticePositions [11] = 0.5f * new Vector3 (1, 1, 1);

		// Hinten
		_VerticePositions [12] = 0.5f * new Vector3 (1, -1, 1);
		_VerticePositions [13] = 0.5f * new Vector3 (-1, -1, 1);
		_VerticePositions [14] = 0.5f * new Vector3 (1, 1, 1);
		_VerticePositions [15] = 0.5f * new Vector3 (-1, 1, 1);

		// Links
		_VerticePositions [16] = 0.5f * new Vector3 (-1, -1, 1);
		_VerticePositions [17] = 0.5f * new Vector3 (-1, -1, -1);
		_VerticePositions [18] = 0.5f * new Vector3 (-1, 1, 1);
		_VerticePositions [19] = 0.5f * new Vector3 (-1, 1, -1);

		// Unten
		_VerticePositions [20] = 0.5f * new Vector3 (-1, -1, 1);
		_VerticePositions [21] = 0.5f * new Vector3 (1, -1, 1);
		_VerticePositions [22] = 0.5f * new Vector3 (-1, -1, -1);
		_VerticePositions [23] = 0.5f * new Vector3 (1, -1, -1);

		// Vorne
		_Normals [0] = new Vector3 (0,0,-1);
		_Normals [1] = new Vector3 (0,0,-1);
		_Normals [2] = new Vector3 (0,0,-1);
		_Normals [3] = new Vector3 (0,0,-1);

		//Rechts
		_Normals [4] = new Vector3 (1,0,0);
		_Normals [5] = new Vector3 (1,0,0);
		_Normals [6] = new Vector3 (1,0,0);
		_Normals [7] = new Vector3 (1,0,0);

		//Oben
		_Normals [8] = new Vector3 (0,1,0);
		_Normals [9] = new Vector3 (0,1,0);
		_Normals [10] = new Vector3 (0,1,0);
		_Normals [11] = new Vector3 (0,1,0);

		// Hinten
		_Normals [12] = new Vector3 (0,0,1);
		_Normals [13] = new Vector3 (0,0,1);
		_Normals [14] = new Vector3 (0,0,1);
		_Normals [15] = new Vector3 (0,0,1);

		//Links
		_Normals [16] = new Vector3 (-1,0,0);
		_Normals [17] = new Vector3 (-1,0,0);
		_Normals [18] = new Vector3 (-1,0,0);
		_Normals [19] = new Vector3 (-1,0,0);

		//Unten
		_Normals [20] = new Vector3 (0,-1,0);
		_Normals [21] = new Vector3 (0,-1,0);
		_Normals [22] = new Vector3 (0,-1,0);
		_Normals [23] = new Vector3 (0,-1,0);

		

		// Vorderseite
		_TriangleIndices [0] = 0;
		_TriangleIndices [1] = 2;
		_TriangleIndices [2] = 1;
		
		_TriangleIndices [3] = 1;
		_TriangleIndices [4] = 2;
		_TriangleIndices [5] = 3;

		// Rechte Seite
		_TriangleIndices [6] = 4;
		_TriangleIndices [7] = 6;
		_TriangleIndices [8] = 5;
		
		_TriangleIndices [9] = 5;
		_TriangleIndices [10] = 6;
		_TriangleIndices [11] = 7;

		// Oberseite
		_TriangleIndices [12] = 8;
		_TriangleIndices [13] = 10;
		_TriangleIndices [14] = 9;
		
		_TriangleIndices [15] = 9;
		_TriangleIndices [16] = 10;
		_TriangleIndices [17] = 11;

		// Rückseite
		_TriangleIndices [18] = 12;
		_TriangleIndices [19] = 14;
		_TriangleIndices [20] = 13;
		
		_TriangleIndices [21] = 13;
		_TriangleIndices [22] = 14;
		_TriangleIndices [23] = 15;

		// Linke Seite
		_TriangleIndices [24] = 16;
		_TriangleIndices [25] = 18;
		_TriangleIndices [26] = 17;
		
		_TriangleIndices [27] = 17;
		_TriangleIndices [28] = 18;
		_TriangleIndices [29] = 19;

		// UnterSeite
		_TriangleIndices [30] = 20;
		_TriangleIndices [31] = 22;
		_TriangleIndices [32] = 21;
		
		_TriangleIndices [33] = 21;
		_TriangleIndices [34] = 22;
		_TriangleIndices [35] = 23;


		
		
		_CubeMesh.vertices = _VerticePositions;
		_CubeMesh.normals = _Normals;
		//_CubeMesh.uv = _TexCoords;
		
		_CubeMesh.triangles = _TriangleIndices;
		
		
		m_MeshFilter.mesh = _CubeMesh;
		
	}
	
	// Update is called once per frame
	void Update () {
		
	}
}
