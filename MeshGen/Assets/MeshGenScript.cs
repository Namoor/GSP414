using UnityEngine;
using System.Collections;

public class MeshGenScript : MonoBehaviour {

	public MeshFilter m_MeshFilter;

	// Use this for initialization
	void Start () {
	
		Mesh _CubeMesh = new Mesh ();

		// Mesh mit daten befüllen

		Vector3[] _VerticePositions = new Vector3[4];
		Vector3[] _Normals = new Vector3[4]; // Eine Normal für jede Vertice
		Vector2[] _TexCoords = new Vector2[4]; // Eine TexCoord für jede Vertice

		int[] _TriangleIndices = new int[6]; // 3 Indices pro Triangle


		//Vertices anlegen
		_VerticePositions [0] = new Vector3 (-1,0.0f,-1);
		_VerticePositions [1] = new Vector3 (1,0.0f,-1);
		_VerticePositions [2] = new Vector3 (-1,0.0f,1);
		_VerticePositions [3] = new Vector3 (1,0.0f,1);

		//Normals anlegen
		_Normals [0] = new Vector3 (0, 1, 0);
		_Normals [1] = new Vector3 (0, 1, 0);
		_Normals [2] = new Vector3 (0, 1, 0);
		_Normals [3] = new Vector3 (0, 1, 0);
		
		_TexCoords [0] = new Vector2 (0, 0);
		_TexCoords [1] = new Vector2 (1, 0);
		_TexCoords [2] = new Vector2 (0, 1);
		_TexCoords [3] = new Vector2 (1, 1);

		//Triangle 1
		_TriangleIndices [0] = 0;
		_TriangleIndices [1] = 2;
		_TriangleIndices [2] = 3;


		// Triangle 2
		_TriangleIndices [3] = 0;
		_TriangleIndices [4] = 3;
		_TriangleIndices [5] = 1;



		
		_CubeMesh.vertices = _VerticePositions;
		_CubeMesh.normals = _Normals;
		_CubeMesh.uv = _TexCoords;

		_CubeMesh.triangles = _TriangleIndices;


		m_MeshFilter.mesh = _CubeMesh;

	}
	
	// Update is called once per frame
	void Update () {
	
	}
}
